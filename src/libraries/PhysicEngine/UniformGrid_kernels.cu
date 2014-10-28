
// <<<<<<<<<< includes >>>>>>>>>> //
#include <cuda.h>
#include <cuda_runtime.h>
#include <device_launch_parameters.h>

#include <glm\glm.hpp>

//resetGrid
void resetGrid(int* countGrid, int4* indexGrid){

	//blocks und threads berechn.

	resetGridC<<< numBlocks, numThreads >>>(countGrid, IndexGrid);
}

__global__ void resetGridC(int* countGrid, int4* indexGrid){

	//...
	//unsigned int flatGridIndex = get_global_id(0);
	int i = blockDim.x * blockIdx.x + threadIdx.x;

	countGrid[i] = 0;
	indexGrid[i].x = -1;
	indexGrid[i].y = -1;
	indexGrid[i].z = -1;
	indexGrid[i].w = -1;
}

//updateGRid
void updateGrid(int* contGrid, int4* indexGrid, glm::vec3* pPos, glm::vec3 gridMinPosVec, float voxelSL, int gridSL, glm::vec3* pGridIndex){

	//blocks und threads berechn.

	updateGridC <<< numBlocks, numThreads >>>(contGrid,indexGrid,pPos,gridMinPosVec,voxelSL,gridSL,pGridIndex);
}

__global__ void updateGridC(int* contGrid, int4* indexGrid, glm::vec3* pPos, glm::vec3 gridMinPosVec, float voxelSL, int gridSL, glm::vec3* pGridIndex){

	//unsigned int particleIndex = get_global_id(0);
	int pi = blockDim.x * blockIdx.x + threadIdx.x;

	pGridIndex[pi].x = (int)((pPos[pi].x - gridMinPosVec.x) / voxelSL);
	pGridIndex[pi].y = (int)((pPos[pi].y - gridMinPosVec.y) / voxelSL);
	pGridIndex[pi].z = (int)((pPos[pi].z - gridMinPosVec.z) / voxelSL);

	bool validIndex = (pGridIndex[pi].x > 0) &&
		(pGridIndex[pi].x < gridSL - 1) &&
		(pGridIndex[pi].y > 0) &&
		(pGridIndex[pi].y < gridSL - 1) &&
		(pGridIndex[pi].z > 0) &&
		(pGridIndex[pi].z < gridSL - 1);

	if (validIndex) {
		int xStride = gridSL * gridSL;
		int yStride = gridSL;
		int flatGridIndex = pGridIndex[pi].x*xStride +
			pGridIndex[pi].y * yStride +
			pGridIndex[pi].z;

		//
		int particlesInCell = atomic_inc(&countGrid[flatGridIndex]);	//?
		//

		if (particlesInCell == 3) {
			indexGrid[flatGridIndex].w = pi;
		}
		else if (particlesInCell == 2) {
			indexGrid[flatGridIndex].z = pi;
		}
		else if (particlesInCell == 1) {
			indexGrid[flatGridIndex].y = pi;
		}
		else if (particlesInCell == 0) {
			indexGrid[flatGridIndex].x = pi;
		}
	}
}