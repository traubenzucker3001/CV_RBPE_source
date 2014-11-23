
// <<<<<<<<<< includes >>>>>>>>>> //
#include "Particle.h"
#include "UniformGrid.h"
#include "World.h"
#include "Cuda.h"

//link fix try 4
extern World* world;
extern Cuda* cuda;

int Particle::indexCount = 0;

Particle::Particle(glm::vec3 posIN, float massIN){

	cout << "part: part constr called!" << endl; //zum test

	position = posIN;
	mass = massIN;
	velocity = glm::vec3(0,0,0);
	force = glm::vec3(0,0,0);

	//gridIndex = ;
	partIndex = 0;
}

Particle::~Particle(){

	//no pointers to clear!
}

//TODO funcs
glm::vec3 Particle::calculateForces(){

	cout << "part: calcForces called!" << endl; //zum test

	//mehrere schritte zusammenfassen
	force = glm::vec3(0,0,0);

	//-----part1-----
	//calculateCollisionForces();		//war auskommentiert

	float partR = world->getPartRadius();
	float worldS = world->getWorldSize();
	float springC = world->getSpringCoeff();
	float dampC = world->getDampCoeff();

	//-----part2-----
	//calculateCollisionForcesWithGrid();
	if (UniformGrid::getInstance()->isValidIndex(gridIndex) == true) {
		int* neighborParticleIndices = UniformGrid::getInstance()->getNeighborPartIndices(gridIndex);

		int ppv = UniformGrid::getInstance()->getPartPerVoxel();
		for (int i=0; i<ppv*27; i++) {
			int neighborIndex = neighborParticleIndices[i];
			if (neighborIndex != -1 && neighborIndex != this->partIndex) {

				//body oder all particles?!	//müsste alle sein
				Particle** temp = world->getAllParticles();
				Particle* neighbors = temp[neighborIndex];
				glm::vec3 jPos = neighbors->getPosition();

				glm::vec3 distance;
				distance.x = jPos.x - position.x;
				distance.y = jPos.y - position.y;
				distance.z = jPos.z - position.z;

				float absDistance = sqrt(distance.x*distance.x + distance.y*distance.y + distance.z*distance.z);

				//float partR = World::getInstance()->getPartRadius();
				//float springC = World::getInstance()->getSpringCoeff();
				//float dampC = World::getInstance()->getDampCoeff();
				if (absDistance + 0.00001f < (2.0f * partR)) {
					force.x = force.x - springC*(2.0f*partR - absDistance)*(distance.x/absDistance);
					force.y = force.y - springC*(2.0f*partR - absDistance)*(distance.y/absDistance);
					force.z = force.z - springC*(2.0f*partR - absDistance)*(distance.z/absDistance);

					glm::vec3 jVel = neighbors->getVelocity();
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
		delete neighborParticleIndices;
	}

	//-----part3-----
	//calculateBoundaryForces();
	bool collisionOccured = false;

	// Ground collision
	if (position.y-partR < 0.0f) {
		collisionOccured = true;
		force.y = force.y + springC*(partR - position.y);
	}

	// X-axis Wall Collision
	if (position.x-partR < -worldS) {
		collisionOccured = true;
		force.x = force.x + springC*(-worldS - position.x + partR);
	} else if (position.x+partR > worldS) {
		collisionOccured = true;
		force.x = force.x + springC*(worldS - position.x - partR);
	}

	// Z-axis Wall Collision
	if (position.z-partR < -worldS) {
		collisionOccured = true;
		force.z = force.z + springC*(-worldS - position.z + partR);
	} else if (position.z+partR > worldS) {
		collisionOccured = true;
		force.z = force.z + springC*(worldS - position.z - partR);
	}

	// Damping
	if (collisionOccured) {
		force.x = force.x - dampC*velocity.x;
		force.y = force.y - dampC*velocity.y;
		force.z = force.z - dampC*velocity.z;
	}

	return force;
}

void Particle::updateVeloc(glm::vec3 bodyPosition, glm::vec3 bodyVelocity, glm::vec3 bodyAngularVelocity){

	cout << "part: updateVeloc called!" << endl; //zum test

	velocity = glm::vec3(0,0,0);

	float scalar = sqrt(bodyAngularVelocity.x*bodyAngularVelocity.x +
						bodyAngularVelocity.y*bodyAngularVelocity.y +
						bodyAngularVelocity.z*bodyAngularVelocity.z);

	if (scalar > 0.0f) {
		scalar *= scalar;
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

	cout << "part: appRot called!" << endl; //zum test

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
}

void Particle::populateArray(){

	cout << "part: popuArr called!" << endl; //zum test

	partIndex = indexCount;

	//body oder all part. array ?!	//müsste eig all sein
	Particle** allP = world->getAllParticles();
	allP[indexCount] = this;	//müsste doch eig durch setter befüllt werden?!, hier wird ja nur referenz und nicht das wirkliche array beschrieben. könnte noch an mehreren stellen falsch sein, vllt array public machen
	indexCount++;
}

void Particle::reset(float* oldBodyPos, float* newBodyPos){

	//...
}

void Particle::updateGridIndex(){

	cout << "part: update GridIndex called!" << endl; //zum test

	float gmp = UniformGrid::getInstance()->getGridMinPos();
	float vS = UniformGrid::getInstance()->getVoxelSize();

	//int cast benötigt?!	//(int)
	gridIndex.x = ((position.x - gmp)/vS);
	gridIndex.y = ((position.y - vS)/vS);
	gridIndex.z = ((position.z - gmp)/vS);
}

void Particle::updateCUDArray(int particleIndex){

	cout << "part: updateCudArr called!" << endl; //zum test

	int i = particleIndex;
	//TODO
	cuda->h_pMass[i] = mass;

	cuda->h_pPos[i] = position;
	cuda->h_pVeloc[i] = velocity;
	cuda->h_pForce[i] = force;
}

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
