
// <<<<<<<<<< includes >>>>>>>>>> //
#include "Particle.h"
#include "UniformGrid.h"
#include "World.h"
#include "Cuda.h"
#include "DemoApp\Demo.h"

//link fix try 4
extern World* world;
extern Cuda* cuda;
extern Demo* demo;

int Particle::indexCount = 0;

Particle::Particle(glm::vec3 posIN, float massIN){

	//cout << "part: part constr called!" << endl; //zum test

	position = posIN;
	/*
	cout << "-constr pos: " << position.x << endl; //zum debuggen
	cout << "-constr pos: " << position.y << endl; //zum debuggen
	cout << "-constr pos: " << position.z << endl; //zum debuggen
	*/
	mass = massIN;
	velocity = glm::vec3(0,0,0);
	force = glm::vec3(0,0,0);

	gridIndex = glm::ivec3(0,0,0);
	partIndex = 0;

	partNode = new CVK::Node();
	partNode->setGeometry(demo->partGeometry);
	partNode->setMaterial((demo->partMaterial));
	partNode->setModelMatrix(glm::translate(glm::mat4(1.0f), position));
	demo->partRoot->addChild(partNode);
	//cout << "-mass: " << mass << endl; //zum debuggen
}

Particle::~Particle(){

	//no pointers to clear!
	delete partNode;
}

glm::vec3 Particle::calculateForces(bool wgIN){		// TODO debugging: cpu vers - partikel reagieren nicht aufeinander, nachbarfindung oder kraft berechnung nicht korrekt

	//cout << "part: calcForces called!" << endl; //zum test

	//mehrere schritte zusammenfassen
	force = glm::vec3(0,0,0);

	float partR = world->getPartRadius();
	float worldS = world->getWorldSize();
	float springC = world->getSpringCoeff();
	float dampC = world->getDampCoeff();

	//cout << "1pFy: " << force.y << endl;	//zum debuggen
	//-----part1-----
	//calculateCollisionForces();		//siehe anhang (unten), ohne gitter
	//bool wg = world->isWithGrid();
	if (wgIN == false){
		cout << "part: calcForces without grid!" << endl; //zum test
		int numP = world->getAllPartNum();
		for (int j = 0; j<numP; j++) {
			if (j != this->partIndex) {

				glm::vec3 jPos = world->allParticles[j]->getPosition();

				glm::vec3 distance;
				distance.x = jPos.x - position.x;
				distance.y = jPos.y - position.y;
				distance.z = jPos.z - position.z;

				float absDistance = sqrt(distance[0] * distance[0] +
										 distance[1] * distance[1] +
										 distance[2] * distance[2]);

				if (absDistance + 0.00001f < (2.0f * partR)) {
					force.x = force.x - springC*(2.0f*partR - absDistance)*(distance.x / absDistance);
					force.y = force.y - springC*(2.0f*partR - absDistance)*(distance.y / absDistance);
					force.z = force.z - springC*(2.0f*partR - absDistance)*(distance.z / absDistance);

					glm::vec3 jVel = world->allParticles[j]->getVelocity();
					glm::vec3 relativeVelocity;
					relativeVelocity.x = jVel.x - velocity.x;
					relativeVelocity.y = jVel.y - velocity.y;
					relativeVelocity.z = jVel.z - velocity.z;

					force.x = force.x + dampC*relativeVelocity.x;
					force.y = force.y + dampC*relativeVelocity.y;
					force.z = force.z + dampC*relativeVelocity.z;
				}
			}
		}
	}
	/*
	float partR = world->getPartRadius();
	float worldS = world->getWorldSize();
	float springC = world->getSpringCoeff();
	float dampC = world->getDampCoeff();
	*/
	//-----part2-----
	//calculateCollisionForcesWithGrid();
	//cout << "gridIndex: " << gridIndex.x << ", " << gridIndex.y << ", " << gridIndex.z << endl;	//zum debuggen
	if (wgIN == true){
		cout << "part: calcForces with grid!" << endl; //zum test
		if (UniformGrid::getInstance()->isValidGridIndex(gridIndex) == true) {
			//cout << "collwithpart!" << endl;	//zum debuggen
			int* neighborParticles = UniformGrid::getInstance()->getNeighborPartIndices(gridIndex);
			int ppv = UniformGrid::getInstance()->getPartPerVoxel();
		
			int target = ppv * 27;
			for (int i=0; i<target; i++) {
				int neighborIndex = neighborParticles[i];
				if (neighborIndex != -1 && neighborIndex != this->partIndex) {
	
					Particle* neighbor = world->allParticles[neighborIndex];
					glm::vec3 jPos = neighbor->getPosition();
					//glm::vec3 jPos = world->allParticles[neighborIndex]->position;

					/*
					cout << "position: " << position.x << ", " << position.y << ", " << position.z << endl;	//zum debuggen
					cout << "jPos: " << jPos.x << ", " << jPos.y << ", " << jPos.z << endl;	//zum debuggen
					*/

					glm::vec3 distance;
					distance.x = jPos.x - position.x;
					distance.y = jPos.y - position.y;
					distance.z = jPos.z - position.z;
	
					float absDistance = sqrt(distance.x*distance.x + distance.y*distance.y + distance.z*distance.z);

					if (absDistance + 0.00001f < (2.0f * partR)) {
						force.x = force.x - springC*(2.0f*partR - absDistance)*(distance.x/absDistance);
						force.y = force.y - springC*(2.0f*partR - absDistance)*(distance.y/absDistance);
						force.z = force.z - springC*(2.0f*partR - absDistance)*(distance.z/absDistance);

						//glm::vec3 jVel = neighbors->getVelocity();
						glm::vec3 jVel = neighbor->getVelocity();
						//glm::vec3 jVel = world->allParticles[neighborIndex]->velocity;

						glm::vec3 relativeVelocity;
						relativeVelocity.x = jVel.x - velocity.x;
						relativeVelocity.y = jVel.y - velocity.y;
						relativeVelocity.z = jVel.z - velocity.z;

						force.x = force.x + dampC*relativeVelocity.x;
						force.y = force.y + dampC*relativeVelocity.y;
						force.z = force.z + dampC*relativeVelocity.z;
					}
				}
			}
			delete neighborParticles;
			//cout << "2pFy: " << force.y << endl;	//zum debuggen
		}
	}

	//-----part3-----
	//calculateBoundaryForces();
	bool collisionOccured = false;

	// Ground collision
	if (position.y-partR < 0.0f) {
		//cout << "groundcoll" << endl;	//zum debuggen
		collisionOccured = true;
		force.y = force.y + springC*(partR - position.y);
	}

	// X-axis Wall Collision
	if (position.x-partR < -worldS) {
		//cout << "xwallcoll" << endl;	//zum debuggen
		collisionOccured = true;
		force.x = force.x + springC*(-worldS - position.x + partR);
	} else if (position.x+partR > worldS) {
		//cout << "xwallcoll" << endl;	//zum debuggen
		collisionOccured = true;
		force.x = force.x + springC*(worldS - position.x - partR);
	}

	// Z-axis Wall Collision
	if (position.z-partR < -worldS) {
		//cout << "zwallcoll" << endl;	//zum debuggen
		collisionOccured = true;
		force.z = force.z + springC*(-worldS - position.z + partR);
	} else if (position.z+partR > worldS) {
		//cout << "xwallcoll" << endl;	//zum debuggen
		collisionOccured = true;
		force.z = force.z + springC*(worldS - position.z - partR);
	}

	// Damping
	if (collisionOccured) {
		force.x = force.x - dampC*velocity.x;
		force.y = force.y - dampC*velocity.y;
		force.z = force.z - dampC*velocity.z;
	}
	//cout << "3pFy: " << force.y << endl;	//zum debuggen
	return force;
}

void Particle::updateVeloc(glm::vec3 bodyPosition, glm::vec3 bodyVelocity, glm::vec3 bodyAngularVelocity){

	//cout << "part: updateVeloc called!" << endl; //zum test

	velocity = glm::vec3(0,0,0);

	float scalar = sqrt(bodyAngularVelocity.x*bodyAngularVelocity.x +
						bodyAngularVelocity.y*bodyAngularVelocity.y +
						bodyAngularVelocity.z*bodyAngularVelocity.z);

	if (scalar > 0.0f) {
		scalar = scalar * scalar;
		glm::vec3 relativePosition = glm::vec3( position.x - bodyPosition.x,
												position.y - bodyPosition.y,
												position.z - bodyPosition.z );

		scalar = (bodyAngularVelocity.x*relativePosition.x +
				  bodyAngularVelocity.y*relativePosition.y +
				  bodyAngularVelocity.z*relativePosition.z) / scalar;

		glm::vec3 term;
		term.x = relativePosition.x - bodyAngularVelocity.x * scalar;
		term.y = relativePosition.y - bodyAngularVelocity.y * scalar;
		term.z = relativePosition.z - bodyAngularVelocity.z*scalar;

		velocity.x = (bodyAngularVelocity.y * term.z - bodyAngularVelocity.z * term.y);
		velocity.y = (bodyAngularVelocity.z * term.x - bodyAngularVelocity.x * term.z);
		velocity.z = (bodyAngularVelocity.x * term.y - bodyAngularVelocity.y * term.x);
	}
	velocity.x = velocity.x + bodyVelocity.x;
	velocity.y = velocity.y + bodyVelocity.y;
	velocity.z = velocity.z + bodyVelocity.z;

}

void Particle::applyRot(glm::mat3 rotatMatrix, glm::vec3 relatPos, glm::vec3 bodyPos){

	//cout << "part: appRot called!" << endl; //zum test

	position.x = relatPos.x*rotatMatrix[0].x +
			relatPos.y*rotatMatrix[0].y +
			relatPos.z*rotatMatrix[0].z;

	position.y = relatPos.x*rotatMatrix[1].x +
			relatPos.y*rotatMatrix[1].y +
			relatPos.z*rotatMatrix[1].z;

	position.z = relatPos.x*rotatMatrix[2].x +
			relatPos.y*rotatMatrix[2].y +
			relatPos.z*rotatMatrix[2].z;

	position.x = position.x + bodyPos.x;
	position.y = position.y + bodyPos.y;
	position.z = position.z + bodyPos.z;
	//cout << "partpos: " << position.x << ", " << position.y << ", " << position.z << endl;	//zum debuggen

	//modelmatrix von node aktualisieren, richtig an dieser stelle ??!
	glm::mat4 modMat = glm::mat4();	//<--TODO-
	partNode->setModelMatrix(modMat);
}

void Particle::populateArray(){

	//cout << "part: popuArr called!" << endl; //zum test

	partIndex = indexCount;

	world->allParticles[indexCount] = this;
	indexCount++;
}

void Particle::reset(float* oldBodyPos, float* newBodyPos){

	//...
}

void Particle::updateGridIndex(){

	//cout << "part: update GridIndex called!" << endl; //zum test

	float gmp = UniformGrid::getInstance()->getGridMinPos();
	float vS = UniformGrid::getInstance()->getVoxelSize();

	//cout << "position: " << position.x << ", " << position.y << ", " << position.z << endl;	//zum debuggen
	gridIndex.x = (int)((position.x - gmp)/vS);		//<--- da liegt ein fehler !!! (cpu vers)
	gridIndex.y = (int)((position.y - vS) / vS);
	//gridIndex.y = ((position.y - gmp) / vS);
	gridIndex.z = (int)((position.z - gmp) / vS);
	//cout << "gridIndex: " << gridIndex.x << ", " << gridIndex.y << ", " << gridIndex.z << endl;	//zum debuggen
}

void Particle::updateCUDArray(int particleIndex){

	//cout << "part: updateCudArr called!" << endl; //zum test

	int i = particleIndex;
	cuda->h_pMass[i] = mass;
	cuda->h_pPos[i] = position;
	cuda->h_pVeloc[i] = velocity;
	cuda->h_pForce[i] = force;
}

//-----"anhang"-----

//calculateCollisionForces();	//ohne gitter
/*
for (int j=0; j<totalNumberOfParticles; j++) {
	if (j != this->particleIndex) {

		float* jPos = particles[j]->getPosition();

		float distance[3];
		distance[0] = jPos[0] - position[0];
		distance[1] = jPos[1] - position[1];
		distance[2] = jPos[2] - position[2];

		float absDistance = sqrt(distance[0]*distance[0] +
					distance[1]*distance[1] +
					distance[2]*distance[2]);

		if (absDistance + 0.00001f < (2.0f * particleRadius)) {
			force[0] = force[0] - springCoefficient*(2.0f*particleRadius - absDistance)*(distance[0]/absDistance);
			force[1] = force[1] - springCoefficient*(2.0f*particleRadius - absDistance)*(distance[1]/absDistance);
			force[2] = force[2] - springCoefficient*(2.0f*particleRadius - absDistance)*(distance[2]/absDistance);

			float* jVel = particles[j]->getVelocity();
			float relativeVelocity[3];
			relativeVelocity[0] = jVel[0] - velocity[0];
			relativeVelocity[1] = jVel[1] - velocity[1];
			relativeVelocity[2] = jVel[2] - velocity[2];

			force[0] += dampingCoefficient*relativeVelocity[0];
			force[1] += dampingCoefficient*relativeVelocity[1];
			force[2] += dampingCoefficient*relativeVelocity[2];
		}
	}
}
*/
