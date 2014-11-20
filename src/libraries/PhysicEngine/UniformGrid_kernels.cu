/*
// <<<<<<<<<< includes >>>>>>>>>> //
#include <cuda.h>
#include <cuda_runtime.h>
#include <device_launch_parameters.h>

#include <glm\glm.hpp>

#include "UniformGrid.h"
#include "World.h"

/*	Änderung damit atomicInc funktioniert: 
	UniformGrid_kernel.cu --> Eigenschaften --> Konfigurationseigenschaften --> CUDA C/C++ --> Device --> Code Generation
	von "compute_20,sm_20" auf "compute_13,sm_13" geändert	*/

//Round a / b to nearest higher integer value
/*int uNearHighVal(int a, int b){
	return (a % b != 0) ? (a / b + 1) : (a / b);
}	//nicht in jedem cu eine funktion, irgwo für alle erreichbar machen


__global__ void resetGridC(int* countGrid, glm::vec4* indexGrid){

	//cout bzw. printf() in kernel möglich?!
	//ja, siehe --> cuda samples - 0_simple - simpleprintf

	//...
	//unsigned int flatGridIndex = get_global_id(0);
	int i = blockDim.x * blockIdx.x + threadIdx.x;

	countGrid[i] = 0;
	indexGrid[i].x = -1;
	indexGrid[i].y = -1;
	indexGrid[i].z = -1;
	indexGrid[i].w = -1;
}

//resetGrid
void resetGrid(int* countGrid, glm::vec4* indexGrid){

	//blocks und threads berechn.
	//int b = World::getInstance()->getAllBodyNum();	//wird bodies oder particle benötigt, oder gitter abhängiges
	//thread pro gitterzelle	//wie komm ich an diese zahl?!
	/*	int g = UniformGrid::getInstance()->getGridSize();
	int blockSize = 64;
	int numThreads = (int)fmin(blockSize, g);
	int numBlocks = uNearHighVal(g, numThreads);
	//geht doch bestimmt auch noch "besser"!!?

	resetGridC <<< numBlocks, numThreads >>>(countGrid, indexGrid);
}


__global__ void updateGridC(int* countGrid, glm::vec4* indexGrid, glm::vec3* pPos, glm::vec3 gridMinPosVec, float voxelSL, int gridSL, glm::vec3* pGridIndex){

	//unsigned int particleIndex = get_global_id(0);
	int pi = blockDim.x * blockIdx.x + threadIdx.x;

	pGridIndex[pi].x = (pPos[pi].x - gridMinPosVec.x) / voxelSL;
	pGridIndex[pi].y = (pPos[pi].y - gridMinPosVec.y) / voxelSL;
	pGridIndex[pi].z = (pPos[pi].z - gridMinPosVec.z) / voxelSL;

	bool validIndex = (pGridIndex[pi].x > 0) &&
		(pGridIndex[pi].x < gridSL - 1) &&
		(pGridIndex[pi].y > 0) &&
		(pGridIndex[pi].y < gridSL - 1) &&
		(pGridIndex[pi].z > 0) &&
		(pGridIndex[pi].z < gridSL - 1);

	if (validIndex) {
		int xStride = gridSL * gridSL;
		int yStride = gridSL;
		int flatGridIndex = (int)pGridIndex[pi].x*xStride +
							(int)pGridIndex[pi].y * yStride +
							(int)pGridIndex[pi].z;

		//todo: zu cuda func
		//int particlesInCell = atomic_inc(&countGrid[flatGridIndex]);	//?
		//
		int n = 4;
		unsigned int* atom = (unsigned int*)(&countGrid[flatGridIndex]);		//potenzielle fehlerquelle
		int particlesInCell = atomicInc(atom,n);
		//

		if (particlesInCell == 3) {
			indexGrid[flatGridIndex].w = (float)pi;
		}
		else if (particlesInCell == 2) {
			indexGrid[flatGridIndex].z = (float)pi;
		}
		else if (particlesInCell == 1) {
			indexGrid[flatGridIndex].y = (float)pi;
		}
		else if (particlesInCell == 0) {
			indexGrid[flatGridIndex].x = (float)pi;
		}
	}
}

//updateGRid
void updateGrid(int* countGrid, glm::vec4* indexGrid, glm::vec3* pPos, glm::vec3 gridMinPosVec, float voxelSL, int gridSL, glm::vec3* pGridIndex){

	//blocks und threads berechn.
	//int b = World::getInstance()->getAllBodyNum();	//wird bodies oder particle benötigt, oder gitter abhängiges
	//thread pro part.
	int p = World::getInstance()->getAllPartNum();
	int blockSize = 64;
	int numThreads = (int)fmin(blockSize, p);
	int numBlocks = uNearHighVal(p, numThreads);
	//geht doch bestimmt auch noch "besser"!!?

	updateGridC <<< numBlocks, numThreads >>>(countGrid, indexGrid, pPos, gridMinPosVec, voxelSL, gridSL, pGridIndex);
}
*/