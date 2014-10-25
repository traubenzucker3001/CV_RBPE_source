
// <<<<<<<<<< includes >>>>>>>>>> //
#include "Particle2.h"
#include "UniformGrid2.h"
#include "World2.h"

int Particle::indexCount = 0;

Particle::Particle(glm::vec3 posIN, float massIN){

	position = posIN;
	mass = massIN;
	velocity = 0;
	force = 0;

	//gridIndex = ;
	partIndex = 0;
}

Particle::~Particle(){

	//...
}

//TODO funcs
float* Particle::calculateForces(){

	//mehrere schritte zusammenfassen
	force = glm::vec3(0,0,0);

	//-----part1-----
	//calculateCollisionForces();		//war auskommentiert

	float partR = World::getInstance()->getPartRadius();
	float worldS = World::getInstance()->getWorldSize();

	//-----part2-----
	//calculateCollisionForcesWithGrid();
	if (isValidIndex(gridIndex) == true) {
		int* neighborParticleIndices = getNeighborParticleIndices(gridIndex);

		int ppv = UniformGrid::getInstance()->getPartPerVoxel();
		for (int i=0; i<ppv*27; i++) {
			int neighborIndex = neighborParticleIndices[i];
			if (neighborIndex != -1 && neighborIndex != this->partIndex) {

				//body oder all particles?!
				Particle* neighbor = particles[neighborIndex];
				float* jPos = neighbor->getPosition();

				float distance[3];
				distance[0] = jPos[0] - position[0];
				distance[1] = jPos[1] - position[1];
				distance[2] = jPos[2] - position[2];

				float absDistance = sqrt(distance[0]*distance[0] + distance[1]*distance[1] + distance[2]*distance[2]);

				//float partR = World::getInstance()->getPartRadius();
				if (absDistance + 0.00001f < (2.0f * partR)) {
					force[0] = force[0] - springCoefficient*(2.0f*partR - absDistance)*(distance[0]/absDistance);
					force[1] = force[1] - springCoefficient*(2.0f*partR - absDistance)*(distance[1]/absDistance);
					force[2] = force[2] - springCoefficient*(2.0f*partR - absDistance)*(distance[2]/absDistance);

					float* jVel = neighbor->getVelocity();
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
		delete neighborParticleIndices;
	}

	//-----part3-----
	//calculateBoundaryForces();
	bool collisionOccured = false;

	// Ground collision
	if (position[1]-partR < 0.0f) {
		collisionOccured = true;
		force[1] += springCoefficient*(partR-position[1]);
	}

	// X-axis Wall Collision
	if (position[0]-partR < -worldS) {
		collisionOccured = true;
		force[0] += springCoefficient*(-worldS - position[0] + partR);
	} else if (position[0]+partR > worldS) {
		collisionOccured = true;
		force[0] += springCoefficient*(worldS - position[0] - partR);
	}

	// Z-axis Wall Collision
	if (position[2]-partR < -worldS) {
		collisionOccured = true;
		force[2] += springCoefficient*(-worldS - position[2] + partR);
	} else if (position[2]+partR > worldS) {
		collisionOccured = true;
		force[2] += springCoefficient*(worldS - position[2] - partR);
	}

	// Damping
	if (collisionOccured) {
		force[0] -= dampingCoefficient*velocity[0];
		force[1] -= dampingCoefficient*velocity[1];
		force[2] -= dampingCoefficient*velocity[2];
	}


	return force;
}

void updateVelocity(float* bodyPosition, float* bodyVelocity, float* bodyAngularVelocity);
void applyRotation(glm::mat3 rotatMatrix, glm::vec3 relatPos, glm::vec3 bodyPos);
void populateArray();

void reset(float* oldBodyPos, float* newBodyPos);

void updateGridIndex();


//-----"anhang"-----

//calculateCollisionForces();
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
