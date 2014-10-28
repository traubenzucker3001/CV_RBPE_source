
// <<<<<<<<<< includes >>>>>>>>>> //
#include <cuda.h>
#include <cuda_runtime.h>
#include <device_launch_parameters.h>

#include <glm\glm.hpp>

//collision detection
void calcCollForces(glm::vec3* pMass, glm::vec3* pPos, glm::vec3* pVeloc, glm::vec3* pForce, float pRadius, float worldS, float springC, float dampC, glm::vec3* pGridIndex, int* countGrid, int4* indexGrid, int gridSL){

	//blocks und threads berechn.

	calcCollForcesC <<< numBlocks, numThreads >>>(pMass,pPos,pVeloc,pForce,pRadius,worldS,springC,dampC,pGridIndex,countGrid,indexGrid,gridSL);
}

__global__ void calcCollForcesC(glm::vec3* pMass, glm::vec3* pPos, glm::vec3* pVeloc, glm::vec3* pForce, float pRadius, float worldS, float springC, float dampC, glm::vec3* pGridIndex, int* countGrid, int4* indexGrid, int gridSL){

	//TODO
	//unsigned int particleIndex = get_global_id(0);
	int pi = blockDim.x * blockIdx.x + threadIdx.x;

	pForce[pi].x = 0.0f;
	pForce[pi].y = 0.0f;
	pForce[pi].z = 0.0f;

	glm::vec3 gridIndex = pGridIndex[pi];

	//Pretend border cell is 1 position inwards to avoid checking outside bounds for neighbors
	//todo: zu cuda func
	//gridIndex = clamp(gridIndex, 1, gridSL - 2);
	gridIndex = glm::clamp(gridIndex, 1, gridSL - 2);

	int xSteps = gridSL*gridSL;
	int ySteps = gridSL;

	int flatGridIndex = gridIndex.x * xSteps + gridIndex.y * ySteps + gridIndex.z;

	int4 neighborCells[27];	//int4??
	int cellIndexJ = 0;

	flatGridIndex -= xSteps;
	flatGridIndex += 2 * ySteps;
	flatGridIndex += 2; //zStride

	for (int x = 0; x<3; x++) {
		flatGridIndex -= 3 * ySteps;

		for (int y = 0; y<3; y++) {
			flatGridIndex -= 3;

			for (int z = 0; z<3; z++) {

				neighborCells[cellIndexJ] = indexGrid[flatGridIndex];
				cellIndexJ++;
				flatGridIndex++;
			}
			flatGridIndex += ySteps;
		}
		flatGridIndex += xSteps;
	}

	for (int j = 0; j<27; j++) {
		//oder glm::...
		int neighborParticles[4] = {
			neighborCells[j].x,
			neighborCells[j].y,
			neighborCells[j].z,
			neighborCells[j].w };

		for (int k = 0; k<4; k++) {
			int otherParticle = neighborParticles[k];
			if ((otherParticle != pi) && (otherParticle != (-1))) {
				glm::vec3 distance = pPos[otherParticle] - pPos[pi];

				float absDistance = sqrt(distance.x*distance.x + distance.y*distance.y + distance.z*distance.z);

				if ((absDistance + 0.000001f) < (2.0f * pRadius)) {
					pForce[pi].x -= springC*
						(pRadius + pRadius - absDistance)*(distance.x / absDistance);
					pForce[pi].y -= springC*
						(pRadius + pRadius - absDistance)*(distance.y / absDistance);
					pForce[pi].z -= springC*
						(pRadius + pRadius - absDistance)*(distance.z / absDistance);

					//glm::vec3
					float3 relativeVelocity = {
						pVeloc[otherParticle].x - pVeloc[pi].x,
						pVeloc[otherParticle].y - pVeloc[pi].y,
						pVeloc[otherParticle].z - pVeloc[pi].z };

					pForce[pi].x += dampC*relativeVelocity.x;
					pForce[pi].y += dampC*relativeVelocity.y;
					pForce[pi].z += dampC*relativeVelocity.z;

				}
			}
		}
	}

	//Boundary forces
	{
		bool collisionOccured = false;
		// Ground collision
		if (pPos[pi].y - pRadius < 0.0f) {
			collisionOccured = true;
			pForce[pi].y += springC*
				(pRadius - pPos[pi].y);
		}

		// X-axis Wall Collision
		if (pPos[pi].x - pRadius < -worldS) {
			collisionOccured = true;
			pForce[pi].x += springC*
				(-worldS - pPos[pi].x + pRadius);

		}
		else if (pPos[pi].x + pRadius > worldS) {
			collisionOccured = true;
			pForce[pi].x += springC*
				(worldS - pPos[pi].x - pRadius);
		}

		// Z-axis Wall Collision
		if (pPos[pi].z - pRadius < -worldS) {
			collisionOccured = true;
			pForce[pi].z += springC*
				(-worldS - pPos[pi].z + pRadius);

		}
		else if (pPos[pi].z + pRadius > worldS) {
			collisionOccured = true;
			pForce[pi].z += springC*
				(worldS - pPos[pi].z - pRadius);
		}

		// Damping
		if (collisionOccured) {
			pForce[pi].x -= dampC*pVeloc[pi].x;
			pForce[pi].y -= dampC*pVeloc[pi].y;
			pForce[pi].z -= dampC*pVeloc[pi].z;
		}
	}
}


//update particles
//wie heiﬂt es in cpu version??
void updatePart(){

	//blocks und threads berechn.

	updatePartC <<< numBlocks, numThreads >>>();
}

__global__ void updatePartC(glm::vec3* rbPos, glm::vec3* rbVeloc, glm::mat3* rbRotMat, glm::vec3* rbAngVeloc, glm::vec3* pPos, glm::vec3* pVeloc, float pRadius){

	//weitere input param
	/*__global float* particleVBO,*/

	//TODO

	//unsigned int particleIndex = get_global_id(0);
	int pi = blockDim.x * blockIdx.x + threadIdx.x;
	int bi = pi / 27;
	int mi = bi * 9;

	glm::vec3 originalRelativePos;
	//Calculate original relative position
	{
		int relativeIndex = pi % 27;

		int xIndex = relativeIndex / 9;
		relativeIndex -= xIndex * 9;

		int yIndex = relativeIndex / 3;
		relativeIndex -= yIndex * 3;

		int zIndex = relativeIndex;

		float space = 2.0f*pRadius;

		xIndex--;
		yIndex--;
		zIndex--;

		originalRelativePos.x = xIndex*space;
		originalRelativePos.y = yIndex*space;
		originalRelativePos.z = zIndex*space;
	}

	//Update particle position
	{

		pPos[pi].x =
			originalRelativePos.x*rbRotMat[mi].x +				//oder doch mi+1 +2 +3 +4 +5 +... ?!
			originalRelativePos.y*rbRotMat[mi].y +
			originalRelativePos.z*rbRotMat[mi].z;

		pPos[pi].y =
			originalRelativePos.x*rbRotMat[mi + 1].x +
			originalRelativePos.y*rbRotMat[mi + 1].y +
			originalRelativePos.z*rbRotMat[mi + 1].z;

		pPos[pi].z =
			originalRelativePos.x*rbRotMat[mi + 2].x +
			originalRelativePos.y*rbRotMat[mi + 2].y +
			originalRelativePos.z*rbRotMat[mi + 2].z;

		pPos[pi].x += rbPos[bi].x;
		pPos[pi].y += rbPos[bi].y;
		pPos[pi].z += rbPos[bi].z;
	}

	//Update particle velocity
	float scalar = sqrt(
		rbAngVeloc[bi].x*rbAngVeloc[bi].x +
		rbAngVeloc[bi].y*rbAngVeloc[bi].y +
		rbAngVeloc[bi].z*rbAngVeloc[bi].z);

	scalar *= scalar;

	pVeloc[pi].x = rbVeloc[bi].x;
	pVeloc[pi].y = rbVeloc[bi].y;
	pVeloc[pi].z = rbVeloc[bi].z;

	if (scalar > 0.0f) {
		float3 relativePosition = {
			pPos[pi].x - rbPos[bi].x,
			pPos[pi].y - rbPos[bi].y,
			pPos[pi].z - rbPos[bi].z };

		float scalar2 = (
			rbAngVeloc[bi].x*relativePosition.x +
			rbAngVeloc[bi].y*relativePosition.y +
			rbAngVeloc[bi].z*relativePosition.z
			) / scalar;

		float3 term = {
			relativePosition.x - rbAngVeloc[bi].x*scalar2,
			relativePosition.y - rbAngVeloc[bi].y*scalar2,
			relativePosition.z - rbAngVeloc[bi].z*scalar2 };

		pVeloc[pi].x += (rbAngVeloc[bi].y*term.z - rbAngVeloc[bi].z*term.y);
		pVeloc[pi].y += (rbAngVeloc[bi].z*term.x - rbAngVeloc[bi].x*term.z);
		pVeloc[pi].z += (rbAngVeloc[bi].x*term.y - rbAngVeloc[bi].y*term.x);
	}

	//Update particle VBO
	//siehe anhang
}


//----- anhang -----
//opencl
	//Update particle VBO
/*
	unsigned int particleVBOIndex = particleIndex * 3;
	particleVBO[particleVBOIndex] = particlePosition[particleIndex].x;
	particleVBO[particleVBOIndex + 1] = particlePosition[particleIndex].y;
	particleVBO[particleVBOIndex + 2] = particlePosition[particleIndex].z;
*/
//