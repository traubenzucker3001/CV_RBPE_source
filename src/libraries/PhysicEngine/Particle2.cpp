
// <<<<<<<<<< includes >>>>>>>>>> //
#include "Particle2.h"
#include "UniformGrid2.h"
#include "World2.h"

int Particle::indexCount = 0;

Particle::Particle(glm::vec3 posIN, float massIN){

	position = posIN;
	mass = massIN;
	velocity = glm::vec3(0,0,0);
	force = glm::vec3(0,0,0);

	//gridIndex = ;
	partIndex = 0;
}

Particle::~Particle(){

	//...
}

//TODO funcs
glm::vec3 Particle::calculateForces(){

	//mehrere schritte zusammenfassen
	force = glm::vec3(0,0,0);

	//-----part1-----
	//calculateCollisionForces();		//war auskommentiert

	float partR = World::getInstance()->getPartRadius();
	float worldS = World::getInstance()->getWorldSize();
	float springC = World::getInstance()->getSpringCoeff();
	float dampC = World::getInstance()->getDampCoeff();

	//-----part2-----
	//calculateCollisionForcesWithGrid();
	if (UniformGrid::getInstance()->isValidIndex(gridIndex) == true) {
		int* neighborParticleIndices = UniformGrid::getInstance()->getNeighborPartIndices(gridIndex);

		int ppv = UniformGrid::getInstance()->getPartPerVoxel();
		for (int i=0; i<ppv*27; i++) {
			int neighborIndex = neighborParticleIndices[i];
			if (neighborIndex != -1 && neighborIndex != this->partIndex) {

				//body oder all particles?!	//müsste alle sein
				Particle* temp = World::getInstance()->getAllParticles();
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
					float relativeVelocity[3];
					relativeVelocity[0] = jVel[0] - velocity[0];
					relativeVelocity[1] = jVel[1] - velocity[1];
					relativeVelocity[2] = jVel[2] - velocity[2];

					force[0] += dampC*relativeVelocity[0];
					force[1] += dampC*relativeVelocity[1];
					force[2] += dampC*relativeVelocity[2];
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
		force[1] += springC*(partR-position[1]);
	}

	// X-axis Wall Collision
	if (position[0]-partR < -worldS) {
		collisionOccured = true;
		force[0] += springC*(-worldS - position[0] + partR);
	} else if (position[0]+partR > worldS) {
		collisionOccured = true;
		force[0] += springC*(worldS - position[0] - partR);
	}

	// Z-axis Wall Collision
	if (position[2]-partR < -worldS) {
		collisionOccured = true;
		force[2] += springC*(-worldS - position[2] + partR);
	} else if (position[2]+partR > worldS) {
		collisionOccured = true;
		force[2] += springC*(worldS - position[2] - partR);
	}

	// Damping
	if (collisionOccured) {
		force[0] -= dampC*velocity[0];
		force[1] -= dampC*velocity[1];
		force[2] -= dampC*velocity[2];
	}


	return force;
}

void Particle::updateVeloc(glm::vec3 bodyPosition, glm::vec3 bodyVelocity, glm::vec3 bodyAngularVelocity){

	velocity = glm::vec3(0,0,0);

	float scalar = sqrt(bodyAngularVelocity[0]*bodyAngularVelocity[0] +
				bodyAngularVelocity[1]*bodyAngularVelocity[1] +
				bodyAngularVelocity[2]*bodyAngularVelocity[2]);

	if (scalar > 0.0f) {
		scalar *= scalar;
		float relativePosition[3] = {position[0]-bodyPosition[0],
			position[1]-bodyPosition[1],
			position[2]-bodyPosition[2]};

		scalar = (bodyAngularVelocity[0]*relativePosition[0] +
				bodyAngularVelocity[1]*relativePosition[1] +
				bodyAngularVelocity[2]*relativePosition[2]) / scalar;

		float term[3];
		term[0] = relativePosition[0] - bodyAngularVelocity[0]*scalar;
		term[1] = relativePosition[1] - bodyAngularVelocity[1]*scalar;
		term[2] = relativePosition[2] - bodyAngularVelocity[2]*scalar;

		velocity[0] = (bodyAngularVelocity[1]*term[2] - bodyAngularVelocity[2]*term[1]);
		velocity[1] = (bodyAngularVelocity[2]*term[0] - bodyAngularVelocity[0]*term[2]);
		velocity[2] = (bodyAngularVelocity[0]*term[1] - bodyAngularVelocity[1]*term[0]);
	}
	velocity.x = velocity.x + bodyVelocity.x;
	velocity.y = velocity.y + bodyVelocity.y;
	velocity.z = velocity.z + bodyVelocity.z;

}

void Particle::applyRot(glm::mat3 rotatMatrix, glm::vec3 relatPos, glm::vec3 bodyPos){

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

	partIndex = indexCount;

	//body oder all part. array ?!	//müsste eig all sein
	Particle* allP = World::getInstance()->getAllParticles();
	allP[indexCount] = this;
	indexCount++;
}

void Particle::reset(float* oldBodyPos, float* newBodyPos){

	//...
}

void Particle::updateGridIndex(){

	float gmp = UniformGrid::getInstance()->getGridMinPos();
	float vS = UniformGrid::getInstance()->getVoxelSize();

	//int cast benötigt?!	//(int)
	gridIndex.x = ((position.x - gmp)/vS);
	gridIndex.y = ((position.y - vS)/vS);
	gridIndex.z = ((position.z - gmp)/vS);
}

void Particle::updateCUDArray(int particleIndex){

	//TODO
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
