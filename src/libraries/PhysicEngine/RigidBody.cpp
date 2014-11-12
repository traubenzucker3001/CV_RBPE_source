//!noch particle array zugriffs fehler!

// <<<<<<<<<< includes >>>>>>>>>> //
#include "RigidBody.h"
#include "DemoApp/Demo.h"
#include "CollisionShape.h"
#include "Box.h"
#include "Sphere.h"

using namespace std;

int RigidBody::count = 0;

RigidBody::RigidBody(float massIN, bool staticIN, bool shapeIN, glm::vec3 posIN){

	cout << "body: rb constr called!" << endl; //zum test

	mass = massIN;
	inverseMass = ((float)1.0)/mass;

	position = posIN;

	isStatic = staticIN;
	if(shapeIN == true){

		//shape = new Sphere();
		cout << "sphere shape not possible!!" << endl;
	}
	else{

		glm::vec3 pOrigIN = posIN;
		float pMassIN = massIN; 
		Particle** bPartIN;
		float numPartIN;
		float halfsizeIN;

		shape = new Box(pOrigIN,pMassIN,bPartIN,numPartIN,halfsizeIN);
	}

	float temp1 = Demo::getInstance()->getTerminalVeloc();
	terminalMom = temp1 * mass;

	count++;
}

RigidBody::~RigidBody(){

	delete shape;
}

void RigidBody::iterate(float duration){

	cout << "body: iterate!" << endl; //zum test

	updateInverseInertiaTensor();

	//performLinearStep(delta);
	//for(int i=0; i<3; i++) {
	velocity.x = linearMomentum.x / mass;
	position.x = position.x + velocity.x*duration;

	velocity.y = linearMomentum.y / mass;
	position.y = position.y + velocity.y*duration;

	velocity.z = linearMomentum.z / mass;
	position.z = position.z + velocity.z*duration;

	//performAngularStep(delta);
	{ //update angular velocity		//was das fürn konstrukt?
		float a = inverseInertiaTensor[0].x;
		float b = inverseInertiaTensor[0].y;
		float c = inverseInertiaTensor[0].z;
		float d = inverseInertiaTensor[1].x;
		float e = inverseInertiaTensor[1].y;
		float f = inverseInertiaTensor[1].z;
		float g = inverseInertiaTensor[2].x;
		float h = inverseInertiaTensor[2].y;
		float i = inverseInertiaTensor[2].z;

		float u = angularMomentum.x;
		float v = angularMomentum.y;
		float w = angularMomentum.z;

		angularVelocity.x = a*u + b*v + c*w;
		angularVelocity.y = d*u + e*v + f*w;
		angularVelocity.z = g*u + h*v + i*w;
	}
	float angularVelocitySize = sqrt(angularVelocity.x*angularVelocity.x +
									 angularVelocity.y*angularVelocity.y +
									 angularVelocity.z*angularVelocity.z);

	if (angularVelocitySize > 0) {

		glm::vec3 rotationAxis = {angularVelocity.x/angularVelocitySize,
			angularVelocity.y/angularVelocitySize,
			angularVelocity.z/angularVelocitySize};

		float rotationAngle = angularVelocitySize * duration;

		float ds = cos(rotationAngle/2.0f);
		float dvx = rotationAxis.x*sin(rotationAngle/2.0f);
		float dvy = rotationAxis.y*sin(rotationAngle/2.0f);
		float dvz = rotationAxis.z*sin(rotationAngle/2.0f);

		float s = rotationQuat.w;
		float vx = rotationQuat.x;
		float vy = rotationQuat.y;
		float vz = rotationQuat.z;

		rotationQuat.w = s*ds - vx*dvx - vy*dvy - vz*dvz;
		rotationQuat.x = ds*vx + s*dvx + dvy*vz - dvz*vy;
		rotationQuat.y = ds*vy + s*dvy + dvz*vx - dvx*vz;
		rotationQuat.z = ds*vz + s*dvz + dvx*vy - dvy*vx;
	}
}

void RigidBody::updateRotMatrix(){

	cout << "body: updateRotMat called!" << endl; //zum test

	//normalizeQuaternion();
	glm::normalize(rotationQuat);

	float w = rotationQuat.w;
	float x = rotationQuat.x;
	float y = rotationQuat.y;
	float z = rotationQuat.z;

	float xx = x * x;
	float yy = y * y;
	float zz = z * z;
	float xy = x * y;
	float xz = x * z;
	float yz = y * z;
	float wx = w * x;
	float wy = w * y;
	float wz = w * z;

	//{ 1.0f-2.0f*(yy+zz), 2.0f*(xy-wz), 2.0f*(xz+wy),
	//  2.0f*(xy+wz), 1.0f-2.0f*(xx+zz), 2.0f*(yz-wx),
	//  2.0f*(xz-wy), 2.0f*(yz+wx), 1.0f-2.0f*(xx+yy) };
	float m1,m2,m3,m4,m5,m6,m7,m8,m9;
	m1 = 1.0f-2.0f*(yy+zz);
	m2 = 2.0f*(xy-wz);
	m3 = 2.0f*(xz+wy);
	m4 = 2.0f*(xy+wz);
	m5 = 1.0f-2.0f*(xx+zz);
	m6 = 2.0f*(yz-wx);
	m7 = 2.0f*(xz-wy);
	m8 = 2.0f*(yz+wx);
	m9 = 1.0f-2.0f*(xx+yy);

	rotationMat = glm::mat3(m1,m2,m3,m4,m5,m6,m7,m8,m9);
}

void RigidBody::updateInverseInertiaTensor(){

	cout << "body: update inverse inertia tensor!!" << endl; //zum test

	float a = rotationMat[0].x;
	float b = rotationMat[0].y;
	float c = rotationMat[0].z;
	float d = rotationMat[1].x;
	float e = rotationMat[1].y;
	float f = rotationMat[1].z;
	float g = rotationMat[2].x;
	float h = rotationMat[2].y;
	float i = rotationMat[2].z;

	float u = initInverseInertTensDiagon.x;
	float v = initInverseInertTensDiagon.y;
	float w = initInverseInertTensDiagon.z;

	inverseInertiaTensor[0].x = u*a*a + b*b*v + c*c*w;
	inverseInertiaTensor[0].y = a*d*u + b*e*v + c*f*w;
	inverseInertiaTensor[0].z = a*g*u + b*h*v + c*i*w;
	inverseInertiaTensor[1].x = a*d*u + b*e*v + c*f*w;
	inverseInertiaTensor[1].y = u*d*d + e*e*v + f*f*w;
	inverseInertiaTensor[1].z = d*g*u + e*h*v + f*i*w;
	inverseInertiaTensor[2].x = a*g*u + b*h*v + c*i*w;
	inverseInertiaTensor[2].y = d*g*u + e*h*v + f*i*w;
	inverseInertiaTensor[2].z = u*g*g + h*h*v + i*i*w;
}

void RigidBody::updatePartValues(){

	cout << "body: update part values!" << endl; //zum test

	updateRotMatrix();
	//runter in collshape greifen
	shape->applyRotToPart(rotationMat);

	//Update particle velocity	//collshape bodypart.
	int numP = shape->getNumOfPart();
	Particle** bodyP = shape->getBodyParticles();
	for (int i=0; i<numP; i++) {
		bodyP[i]->updateVeloc(position, velocity, angularVelocity);
	}
}

void RigidBody::updateMomenta(float duration){

	cout << "body: updateMom called!" << endl; //zum test

	//runter in collshape greifen#
	force = glm::vec3 (0.0f, 0.0f, 0.0f); //reset forces

	//gravity klasse nicht nötig
	float gravity = World::getInstance()->getGravity();
	force.y = force.y + mass * -gravity; //force of gravity

	glm::vec3 torque = glm::vec3(0.0f, 0.0f, 0.0f);

	int numP = shape->getNumOfPart();
	Particle** bodyP = shape->getBodyParticles();
	for (int i=0; i<numP; i++) {
		glm::vec3 particleForce = bodyP[i]->calculateForces();
		force.x += particleForce.x;
		force.y += particleForce.y;
		force.z += particleForce.z;

		glm::vec3 particlePos = bodyP[i]->getPosition();
		glm::vec3 relativePos;
		relativePos.x = particlePos.x - position.x;
		relativePos.y = particlePos.y - position.y;
		relativePos.z = particlePos.z - position.z;

		torque.x = torque.x + relativePos.y * particleForce.z - relativePos.z * particleForce.y;
		torque.y = torque.y + relativePos.z * particleForce.x - relativePos.x * particleForce.z;
		torque.z = torque.z + relativePos.x * particleForce.y - relativePos.y * particleForce.x;
	}

	//for (int i=0; i<3; i++) {
	linearMomentum.x += force.x * duration;
	if (linearMomentum.x > 0.0f) {
		linearMomentum.x = std::min(linearMomentum.x,terminalMom);
	} else {
		linearMomentum.x = std::max(linearMomentum.x,-terminalMom);
	}

	angularMomentum.x += torque.x * duration;

	linearMomentum.y += force.y * duration;
	if (linearMomentum.y > 0.0f) {
		linearMomentum.y = std::min(linearMomentum.y,terminalMom);
	} else {
		linearMomentum.y = std::max(linearMomentum.y,-terminalMom);
	}

	angularMomentum.y += torque.y * duration;

	linearMomentum.z += force.z * duration;
	if (linearMomentum.z > 0.0f) {
		linearMomentum.z = std::min(linearMomentum.z,terminalMom);
	} else {
		linearMomentum.z = std::max(linearMomentum.z,-terminalMom);
	}

	angularMomentum.z += torque.z * duration;
}

void RigidBody::reset(float newPosition){

	//...
}

void RigidBody::updateCUDArray(int bodyIndex){

	cout << "body: updateCudArr called!" << endl; //zum test

	//TODO
}
