
// <<<<<<<<<< includes >>>>>>>>>> //
#include <cuda.h>
#include <cuda_runtime.h>
#include <device_launch_parameters.h>

#include "World.h"

#define GLM_FORCE_CUDA
#include <glm\glm.hpp>

//Round a / b to nearest higher integer value
int nearHighVal(int a, int b){
	return (a % b != 0) ? (a / b + 1) : (a / b);
}

//collision detection	//unter kernel geschoben, funktion muss vor aufruf bekannt sein
/*void calcCollForces(float* pMass, glm::vec3* pPos, glm::vec3* pVeloc, glm::vec3* pForce, float pRadius, float worldS, float springC, float dampC, glm::vec3* pGridIndex, int* countGrid, glm::vec4* indexGrid, int gridSL){

	//blocks und threads berechn.
	int n = World::getInstance()->getAllPartNum();
	int blockSize = 64;
	int numThreads = (int)fmin(blockSize, n);
	int numBlocks = nearHighVal(n, numThreads);

	calcCollForcesC<<< numBlocks, numThreads >>>(pMass,pPos,pVeloc,pForce,pRadius,worldS,springC,dampC,pGridIndex,countGrid,indexGrid,gridSL);
}*/

__global__ void calcCollForcesC(float* pMass, glm::vec3* pPos, glm::vec3* pVeloc, glm::vec3* pForce, float pRadius, float worldS, float springC, float dampC, glm::vec3* pGridIndex, int* countGrid, glm::vec4* indexGrid, int gridSL){

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
	gridIndex = glm::clamp(gridIndex, 1.0f, (float)gridSL - 2.0f);
	int xSteps = gridSL*gridSL;
	int ySteps = gridSL;

	int flatGridIndex = (int)gridIndex.x * xSteps + (int)gridIndex.y * ySteps + (int)gridIndex.z;

	//oder glm::vec4 besser??, dann auf umstellung bei zählweise achten!!
	glm::vec4 neighborCells[27];	//int4??
	int cellIndexJ = 0;

	flatGridIndex = flatGridIndex - xSteps;
	flatGridIndex = flatGridIndex + 2 * ySteps;
	flatGridIndex = flatGridIndex + 2; //zStride

	for (int x = 0; x<3; x++) {
		flatGridIndex = flatGridIndex - 3 * ySteps;

		for (int y = 0; y<3; y++) {
			flatGridIndex = flatGridIndex - 3;

			for (int z = 0; z<3; z++) {

				neighborCells[cellIndexJ] = indexGrid[flatGridIndex];
				cellIndexJ++;
				flatGridIndex++;
			}
			flatGridIndex = flatGridIndex + ySteps;
		}
		flatGridIndex = flatGridIndex + xSteps;
	}

	for (int j = 0; j<27; j++) {
		//oder glm::...
		glm::vec4 neighborParticles = glm::vec4(neighborCells[j].x,
												neighborCells[j].y,
												neighborCells[j].z,
												neighborCells[j].w );

		for (int k = 0; k<4; k++) {
			int otherParticle = (int)neighborParticles[k];
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

//collision detection
void calcCollForces(float* pMass, glm::vec3* pPos, glm::vec3* pVeloc, glm::vec3* pForce, float pRadius, float worldS, float springC, float dampC, glm::vec3* pGridIndex, int* countGrid, glm::vec4* indexGrid, int gridSL){

	//blocks und threads berechn.
	int n = World::getInstance()->getAllPartNum();
	int blockSize = 64;
	int numThreads = (int)fmin(blockSize, n);
	int numBlocks = nearHighVal(n, numThreads);

	calcCollForcesC <<< numBlocks, numThreads >>>(pMass, pPos, pVeloc, pForce, pRadius, worldS, springC, dampC, pGridIndex, countGrid, indexGrid, gridSL);
}

//update particles		//unter kernel geschoben, funktion muss vor aufruf bekannt sein
//wie heißt es in cpu version??
/*void updatePart(glm::vec3* rbPos, glm::vec3* rbVeloc, glm::mat3* rbRotMat, glm::vec3* rbAngVeloc, glm::vec3* pPos, glm::vec3* pVeloc, float pRadius){

	//blocks und threads berechn.
	int n = World::getInstance()->getAllPartNum();
	int blockSize = 64;
	int numThreads = fmin(blockSize, n);
	int numBlocks = nearHighVal(n, numThreads);

	updatePartC <<< numBlocks, numThreads >>>(rbPos, rbVeloc,rbRotMat, rbAngVeloc, pPos, pVeloc,pRadius);
}*/

__global__ void updatePartC(glm::vec3* rbPos, glm::vec3* rbVeloc, glm::mat3* rbRotMat, glm::vec3* rbAngVeloc, glm::vec3* pPos, glm::vec3* pVeloc, float pRadius){

	//weitere input param
	/*__global float* particleVBO,*/

	//TODO

	//unsigned int particleIndex = get_global_id(0);
	int pi = blockDim.x * blockIdx.x + threadIdx.x;
	int bi = pi / 27;
	//int mi = bi * 9;	//*9 nicht nötig wenn glm::mat3!? also mi eig nit nötig

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

		originalRelativePos.x = (float)xIndex * space;
		originalRelativePos.y = yIndex*space;
		originalRelativePos.z = zIndex*space;
	}

	//Update particle position
	{
		glm::mat3 tempRotMat = rbRotMat[bi];
		pPos[pi].x =
			originalRelativePos.x * tempRotMat[0].x +				//oder doch mi+1 +2 +3 +4 +5 +... ?!	//.xyz wollt er nit
			originalRelativePos.y * tempRotMat[0].y +
			originalRelativePos.z * tempRotMat[0].z;

		pPos[pi].y =
			originalRelativePos.x * tempRotMat[1].x +
			originalRelativePos.y * tempRotMat[1].y +
			originalRelativePos.z * tempRotMat[1].z;

		pPos[pi].z =
			originalRelativePos.x * tempRotMat[2].x +
			originalRelativePos.y * tempRotMat[2].y +
			originalRelativePos.z * tempRotMat[2].z;

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

//update particles
//wie heißt es in cpu version??
void updatePart(glm::vec3* rbPos, glm::vec3* rbVeloc, glm::mat3* rbRotMat, glm::vec3* rbAngVeloc, glm::vec3* pPos, glm::vec3* pVeloc, float pRadius){

	//blocks und threads berechn.
	int n = World::getInstance()->getAllPartNum();
	int blockSize = 64;
	int numThreads = (int)fmin(blockSize, n);
	int numBlocks = nearHighVal(n, numThreads);

	updatePartC <<< numBlocks, numThreads >>>(rbPos, rbVeloc, rbRotMat, rbAngVeloc, pPos, pVeloc, pRadius);
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