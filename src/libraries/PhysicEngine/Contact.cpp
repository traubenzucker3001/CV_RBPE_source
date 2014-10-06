#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "Contact.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>


void Contact::setBodyData(RigidBody *oneIN, RigidBody *twoIN, float fricIN, float restiIN) {

	collBodies[0] = oneIN;
	collBodies[1] = twoIN;
	friction = fricIN;
	restitution = restiIN;
}

void Contact::calcInternData(float duration){

	//wenn erster body NULL dann swappen
	if (collBodies[0] == NULL){
		swapBodies();
	}
	//contact basis berechnen
	calcContactBasis();

	//relative contact position zu jedem body
	relatContPos[0] = contactPoint - collBodies[0]->getPosition();
	if (collBodies[1] != NULL) {
		relatContPos[1] = contactPoint - collBodies[1]->getPosition();
	}
	//berechne relative geschwindigkeit der bodies am contact point
	contactVelocity = calcLocalVeloc(0, duration);
	if (collBodies[1] != NULL) {
		contactVelocity = contactVelocity - calcLocalVeloc(1, duration);
	}
	//berechne nötige geschwindigkeitsänderung um kontakt aufzulösen
	calcDesiredVeloc(duration);
}

void Contact::swapBodies(){

	contactNormal = contactNormal * -1.0f;

	RigidBody *temp = collBodies[0];
	collBodies[0] = collBodies[1];
	collBodies[1] = temp;
}

void Contact::calcDesiredVeloc(float duration){

	//???
	//geschwindigkeitslimit
	const static float velocLimit = 0.25;

	//berechne durch letzten frame geschwindigkeit induzierte beschleunigung
	float lastFrameVeloc = collBodies[0]->getLastFrameVeloc() * duration * contactNormal;
	if (collBodies[1] != NULL){
		lastFrameVeloc = lastFrameVeloc - collBodies[1]->getLastFrameVeloc() * duration * contactNormal;
	}
	//wenn velocity sehr klein, restitution limitieren
	float restitu = restitution;
	if (abs(contactVelocity.x) < velocLimit){
		restitu = 0.0;
	}
	//berechne nötige geschwindigkeit
	desiredVelocity = -contactVelocity.x -restitu * (contactVelocity.x - lastFrameVeloc);
}

glm::vec3 Contact::calcLocalVeloc(unsigned int bodyIndex, float duration){

	RigidBody *body = collBodies[bodyIndex];

	//berechne geschwindigkeit an contact point
	glm::vec3 veloc = body->getRotation() % relatContPos[bodyIndex];
	veloc = veloc + body->getVelocity();

	//geschwindigkeit in kontakt koordinaten
	glm::mat3 temp1 = glm::transpose(contactToWorld);
	glm::vec3 contVeloc = temp1 * veloc;

	//berechne von kräften erzeugten geschwindigkeitsbetrag
	glm::vec3 velocAmount = body->getLastFrameVeloc() * duration;

	//geschwindigkeit in kontakt koordinaten
	glm::mat3 temp2 = glm::transpose(contactToWorld);
	velocAmount = temp2 * velocAmount;

	//ignoriere beschleunigung in richtung der contact normal
	velocAmount.x = 0;

	//addiere planare geschwindigkeit
	contVeloc = contVeloc + velocAmount;

	return contVeloc;
}

void Contact::calcContactBasis(){

	glm::vec3 contTangent[2];

	//wenn x-achse oder y-achse näher an z-achse
	if(abs(contactNormal.x) > abs(contactNormal.y)){
		//skalierungsfaktor, damit ergebnis normalisiert ist
	    const float scale1 = 1.0 / sqrt(contactNormal.z * contactNormal.z + contactNormal.x * contactNormal.x);

	    //neue x-achse orthogonal zu welt-y-achse
	    contTangent[0].x = contactNormal.z*scale1;
	    contTangent[0].y = 0;
	    contTangent[0].z = -contactNormal.x*scale1;

	    //neue y-achse ortogonal zu neuer x- und z-achse
	    contTangent[1].x = contactNormal.y*contTangent[0].x;
	    contTangent[1].y = contactNormal.z*contTangent[0].x - contactNormal.x*contTangent[0].z;
	    contTangent[1].z = -contactNormal.y*contTangent[0].x;
	}
	else{
		//skalierungsfaktor, damit ergebnis normalisiert ist
	    const float scale2 = 1.0 / sqrt(contactNormal.z * contactNormal.z + contactNormal.y * contactNormal.y);

	    //neue x-achse orthogonal zu welt-x-achse
	    contTangent[0].x = 0;
	    contTangent[0].y = -contactNormal.z*scale2;
	    contTangent[0].z = contactNormal.y*scale2;

	    //neue y-achse orthogonal zu neuer x- und z-achse
	    contTangent[1].x = contactNormal.y*contTangent[0].z - contactNormal.z*contTangent[0].y;
	    contTangent[1].y = -contactNormal.x*contTangent[0].z;
	    contTangent[1].z = contactNormal.x*contTangent[0].y;
	}
	//erstelle matrix aus 3 vectoren
	contactToWorld[0] = glm::vec3(contactNormal);
	contactToWorld[1] = glm::vec3(contTangent[0]);
	contactToWorld[2] = glm::vec3(contTangent[1]);
}

void Contact::applyImpulse(){

	//???
}

void Contact::applyVelocChange(glm::vec3 velocChange[2], glm::vec3 rotatChange[2]){

	//masse und inertia tensor in welt koordinaten
	glm::mat3 inverseInerTens[2];
	inverseInerTens[0] = collBodies[0]->getInverseInertiaTensorW();

	//berechne impulse für jede kontakt achse
	glm::vec3 contImpulse;

	//wenn reibung = 0
	if (friction == 0.0){
		//erzeuge vektor in welt koordinaten mit geschwindigkeitsänderung pro einheit impuls in richtung contact normal
	    glm::vec3 deltaVelocW1 = relatContPos[0] % contactNormal;
	    deltaVelocW1 = inverseInerTens[0] * deltaVelocW1;
	    deltaVelocW1 = deltaVelocW1 % relatContPos[0];

	    //geschwindigkeitsänderung in kontakt koordinaten
	    float deltaVelocL = deltaVelocW1 * contactNormal;

	    //addiere lineare geschwindigkeitsänderung
	    deltaVelocL = deltaVelocL + collBodies[0]->getInverseMass();

	    //wenn 2. body existiert
	    if (collBodies[1] != NULL){
	    	//inertia tenser für 2. body
	    	inverseInerTens[1] = collBodies[1]->getInverseInertiaTensorW();

	    	//erzeuge vektor wie oben
	        glm::vec3 deltaVelocW2 = relatContPos[1] % contactNormal;
	        deltaVelocW2 = inverseInerTens[1] * deltaVelocW2;
	        deltaVelocW2 = deltaVelocW2 % relatContPos[1];

	        //addiere geschwindigkeitsänderung durch rotation
	        deltaVelocL = deltaVelocL + deltaVelocW2 * contactNormal;

	        //addiere lineare geschwindigkeitsänderung
	         deltaVelocL = deltaVelocL + collBodies[1]->getInverseMass();
	    }
	    //berechne benötigte impulse
	    contImpulse.x = desiredVelocity / deltaVelocL;
	    contImpulse.y = 0;
	    contImpulse.z = 0;
	}

	//sonst, wenn reibung vorhanden
	else{
		float inverseMass = collBodies[0]->getInverseMass();

		//erzeuge schiefsymmetrische matrix für konvertierung zw. linera und angular Anteil
	    glm::mat3 impulseToTorque;
	    impulseToTorque = glm::mat3(0.0, -relatContPos[0].z, relatContPos[0].y,
	    							relatContPos[0].z, 0.0, -relatContPos[0].x,
	    							-relatContPos[0].y, relatContPos[0].x, 0.0);

	    //erzeuge matrix für konvertierung zw. impuls zu geschwindigkeitsänderung in welt koordinaten
	    glm::mat3 deltaVelocW3 = impulseToTorque;
	    deltaVelocW3 = deltaVelocW3 * inverseInerTens[0];
	    deltaVelocW3 = deltaVelocW3 * impulseToTorque;
	    deltaVelocW3 = deltaVelocW3* -1;

	    //wenn 2. body existiert
	    if (collBodies[1]){
	    	//inertia tenser für 2. body
	    	inverseInerTens[1] = collBodies[1]->getInverseInertiaTensorW();

	    	//schiefsymmetrische matrix
	        impulseToTorque = glm::mat3(0.0, -relatContPos[1].z, relatContPos[1].y,
										relatContPos[1].z, 0.0, -relatContPos[1].x,
										-relatContPos[1].y, relatContPos[1].x, 0.0);

	        //berechne geschwindigkeitsänderungsmatrix
	        glm::mat3 deltaVelocW4 = impulseToTorque;
	        deltaVelocW4 = deltaVelocW4 * inverseInerTens[1];
	        deltaVelocW4 = deltaVelocW4 * impulseToTorque;
	        deltaVelocW4 = deltaVelocW4 * -1;

	        //addiere totale delta velocity
	        deltaVelocW3 = deltaVelocW3 + deltaVelocW4;

	        //addiere zu inverser masse
	        inverseMass = inverseMass + collBodies[1]->getInverseMass();
	    }
	    //konvertiere in kontakt koordinaten
	    glm::mat3 deltaVeloc = glm::transpose(contactToWorld);
	    deltaVeloc = deltaVeloc * deltaVelocW3;
	    deltaVeloc = deltaVeloc * contactToWorld;

	    //addiere lineare geschwindigkeitsänderung
	    deltaVeloc[0][0] = deltaVeloc[0][0] + inverseMass;
	    deltaVeloc[1][1] = deltaVeloc[1][1] + inverseMass;
	    deltaVeloc[2][2] = deltaVeloc[2][2] + inverseMass;

	    //invertiere um impuls pro einheit geschwindigkeit zu bekommen
	    glm::vec3 impulseMatrix = glm::inverse(deltaVeloc);

	    //finde zielgeschwindigkeit
	    glm::vec3 targetVeloc(desiredVelocity, -contactVelocity.y, -contactVelocity.z);

	    //finde nötigen impuls zur zielgeschwindigkeit
	    contImpulse = impulseMatrix * targetVeloc;

	    //prüfen ob reibung überstiegen/überwunden
	    float planarImpulse = sqrt(contImpulse.y * contImpulse.y + contImpulse.z * contImpulse.z);
	    if (planarImpulse > contImpulse.x * friction){

	    	//dynamic friction
	    	contImpulse.y = contImpulse.y / planarImpulse;
	    	contImpulse.z = contImpulse.z / planarImpulse;

	    	contImpulse.x = deltaVeloc[0][0] + deltaVeloc[1][0]*friction*contImpulse.y + deltaVeloc[2][0]*friction*contImpulse.z;	//hier könnten mögl. idexe von mat3 vertauscht sein
	    	contImpulse.x = desiredVelocity / contImpulse.x;
	    	contImpulse.y = contImpulse.y * friction * contImpulse.x;
	    	contImpulse.z = contImpulse.z * friction * contImpulse.x;
	    }
	}
	//impulse in welt koordinaten
	glm::vec3 impulse = contactToWorld * contImpulse;

	//aufteilen in linear und angular komponente
	glm::vec3 impulsiveTorque = relatContPos[0] % impulse;
	rotatChange[0] = inverseInerTens[0] * impulsiveTorque;
	velocChange[0].x = velocChange[0].y = velocChange[0].z = 0;
	velocChange[0].x = velocChange[0].x + (impulse.x * collBodies[0]->getInverseMass());
	velocChange[0].y = velocChange[0].y + (impulse.y * collBodies[0]->getInverseMass());
	velocChange[0].z = velocChange[0].z + (impulse.z * collBodies[0]->getInverseMass());

	//änderungen übernehmen
	collBodies[0]->addVelocity(velocChange[0]);
	collBodies[0]->addRotation(rotatChange[0]);

	if (collBodies[1] != NULL){
		//linear und angular änderung feststellen
	    glm::vec3 impulsiveTorque = impulse % relatContPos[1];
	    rotatChange[1] = inverseInerTens[1] * impulsiveTorque;
	    velocChange[1].x = velocChange[1].y = velocChange[1].z = 0;
	    velocChange[1].x = velocChange[1].x + (impulse.x * -collBodies[1]->getInverseMass());
	    velocChange[1].y = velocChange[1].y + (impulse.y * -collBodies[1]->getInverseMass());
	    velocChange[1].z = velocChange[1].z + (impulse.z * -collBodies[1]->getInverseMass());

	    //änderungen übernehmen
	    collBodies[1]->addVelocity(velocChange[1]);
	    collBodies[1]->addRotation(rotatChange[1]);
	}
}

void Contact::applyPosChange(glm::vec3 velocChange[2], glm::vec3 rotatDirection[2], float rotatAmount[2], float penetration){

	float angularLimit = 1000;
	float angularMotion[2],linearMotion[2];
	int index;

	float totalInertia = 0;
	float linearInertia[2];
	float angularInertia[2];

	//inertia von jedem objekt in richtung contact normal
	for (int i = 0; i < 2; i++) {
		if (collBodies[i] != NULL) {
			glm::mat3 inverseInerTens;
			inverseInerTens = collBodies[i]->getInverseInertiaTensorW();

	        //selbe prozedur wie bei reibungsloser geschwindigkeit
			glm::vec3 angularInertiaW = relatContPos[i] % contactNormal;
	        angularInertiaW = inverseInerTens * angularInertiaW;
	        angularInertiaW = angularInertiaW % relatContPos[i];
	        angularInertia[i] = angularInertiaW * contactNormal;

	        //inverse masse als lineare komponente
	        linearInertia[i] = collBodies[i]->getInverseMass();

	        //total inertia aller komponenten
	        totalInertia = totalInertia + linearInertia[i] + angularInertia[i];
		}
	}
	float inverseMass[2];

	totalInertia = angularInertia[0] + collBodies[0]->getInverseMass();

	if(collBodies[1] != NULL){
		inverseMass[1] = angularInertia[1] + collBodies[1]->getInverseMass();
	    totalInertia = totalInertia + inverseMass[1];

	    angularMotion[1] = -penetration * angularInertia[1]/totalInertia;
	    linearMotion[1] = -penetration * collBodies[1]->getInverseMass()/totalInertia;

	    //angulare bewegung limitieren
	    glm::vec3 projection = relatContPos[1];
	    //x*vector.x + y*vector.y + z*vector.z
	    float temp1 = (-relatContPos[1].x * contactNormal.x) + (-relatContPos[1].y * contactNormal.y) + (-relatContPos[1].z * contactNormal.z);
	    projection.x = projection.x + (contactNormal.x * temp1);
	    projection.y = projection.y + (contactNormal.y * temp1);
	    projection.z = projection.z + (contactNormal.z * temp1);

	    //sqrt(x*x+y*y+z*z)
	    float mag1 = sqrt(relatContPos[0].x*relatContPos[0].x + relatContPos[0].y*relatContPos[0].y + relatContPos[0].z*relatContPos[0].z);
	    float max1 = angularLimit * mag1;

	    if(abs(angularMotion[1]) > max1){
	    	float temp = angularMotion[1] + linearMotion[1];
	        angularMotion[1]=angularMotion[1]>0?max1:-max1;	//ändern
	        linearMotion[1]  =temp - angularMotion[1];
	    }
	}
	angularMotion[0] = penetration * angularInertia[0] / totalInertia;
	linearMotion[0] = penetration  *collBodies[0]->getInverseMass() / totalInertia;

	//angulare bewegung limitieren
	glm::vec3 projection = relatContPos[0];
	float temp2 = (-relatContPos[0].x * contactNormal.x) + (-relatContPos[0].y * contactNormal.y) + (-relatContPos[0].z * contactNormal.z);
	projection.x = projection.x + (contactNormal.x * temp2);
	projection.y = projection.y + (contactNormal.y * temp2);
	projection.z = projection.z + (contactNormal.z * temp2);

	//sqrt(x*x+y*y+z*z)
	float mag2 = sqrt(relatContPos[0].x*relatContPos[0].x + relatContPos[0].y*relatContPos[0].y + relatContPos[0].z*relatContPos[0].z);
	float max2 = angularLimit * mag2;

	if(abs(angularMotion[0]) > max2){
		float pp=angularMotion[0]+linearMotion[0];
	    angularMotion[0]=angularMotion[0]>0?max2:-max2;	//ändern
	    linearMotion[0]=pp-angularMotion[0];
	}

	for(index=0;index<2;index++){
		if(collBodies[index]){
			glm::vec3 temp3;
			if(angularMotion[index] != 0.0){
				//kreuzprodukt/vektorprodukt
				temp3 = glm::cross(relatContPos[index], contactNormal);

				glm::mat3 inverseInerTens =  collBodies[index]->getInverseInertiaTensorW();
				rotatDirection[index] = inverseInerTens * temp3;
				rotatAmount[index] = angularMotion[index] / angularInertia[index];

	        //assert(rotatAmount[index]!=((double)0.0));
			}
			else{
				rotatDirection[index].x = rotatDirection[index].y = rotatDirection[index].z = 0;
				rotatAmount[index]=1;
			}

	    velocChange[index] = contactNormal;
	    velocChange[index] = velocChange[index] * (linearMotion[index] / rotatAmount[index]);

	    glm::vec3 pos = collBodies[index]->getPosition();
	    pos.x = pos.x + (contactNormal.x * linearMotion[index]);
	    pos.y = pos.y + (contactNormal.y * linearMotion[index]);
	    pos.z = pos.z + (contactNormal.z * linearMotion[index]);

	    collBodies[index]->setPosition(pos);

	    glm::dquat ori = collBodies[index]->getOrientation();
	    float temp4 = rotatAmount[index] * 0.5;
	    //ori.addScaledVector(rotatDirection[index], temp4);
	    //quaternion + vector
	    //ori = glm::???;
	    collBodies[index]->setOrientation(ori);

		}
	}
}
