
#include <cuda.h>
#include <cuda_runtime.h>

#define GLM_FORCE_CUDA
#include <glm\glm.hpp>

//#include "UniformGrid.h"
#include "World.h"

#include "kernels.cuh"
#include "kernels_impl.cuh"

//link fix try 4
extern World* world;


int nearHighVal(int a, int b){
	return (a % b != 0) ? (a / b + 1) : (a / b);
}

void fillDeviceSymbols(float voxelS, int gridSL, float worldS, float springC, float dampC, float pRadius, float duration, float termVeloc, glm::vec3 gridMinPosVector){
	
	cudaMemcpyToSymbol((const void*)&d_voxelS, &voxelS, sizeof(float));
	
	cudaMemcpyToSymbol((const void*)&d_gridSL, &gridSL, sizeof(int));
	
	cudaMemcpyToSymbol((const void*)&d_worldS, &worldS, sizeof(float));
	
	cudaMemcpyToSymbol((const void*)&d_springC, &springC, sizeof(float));
	
	cudaMemcpyToSymbol((const void*)&d_dampC, &dampC, sizeof(float));
	
	cudaMemcpyToSymbol((const void*)&d_pRadius, &pRadius, sizeof(float));
	
	cudaMemcpyToSymbol((const void*)&d_duration, &duration, sizeof(float));
	
	cudaMemcpyToSymbol((const void*)&d_termVeloc, &termVeloc, sizeof(float));

	//cudaMemcpyToSymbol("d_gridMinPosVector", &h_gridMinPosVector, sizeof(glm::vec3));
	cudaMemcpyToSymbol((const void*)&d_gridMinPosVecX, &gridMinPosVector.x, sizeof(float));
	
	cudaMemcpyToSymbol((const void*)&d_gridMinPosVecY, &gridMinPosVector.y, sizeof(float));
	
	cudaMemcpyToSymbol((const void*)&d_gridMinPosVecZ, &gridMinPosVector.z, sizeof(float));
}


//<<<<<<<<<< uniformgrid kernels >>>>>>>>>>	
void resetGrid(int* gridCounters, glm::ivec4* gridCells, int g){
		
	//blocks und threads berechn.
	//thread pro gitterzelle
	int blockSize = 64;	//64, 256, 512, 1024	
	//int numThreads = (int)fmin(blockSize, g);
	int numThreads = blockSize;
	int numBlocks = nearHighVal(g, numThreads);	
	//int numBlocks = ((g + 1024 -1) / 1024);
	
	//int blocksPerGrid = (numElements + threadsPerBlock - 1) / threadsPerBlock;	//aus vectorAdd

	resetGridC <<< numBlocks, numThreads >>>(gridCounters, gridCells, g);
	cudaThreadSynchronize();
}

void updateGrid(int* gridCounters, glm::ivec4* gridCells, glm::vec3* pPos, glm::ivec3* pGridIndex){	//, float voxelSL, int gridSL , glm::vec3 gridMinPosVec

	//thread pro part.
	int p = world->getAllPartNum();

	int blockSize = 64;
	int numThreads = (int)fmin(blockSize, p);
	int numBlocks = nearHighVal(p, numThreads);

	updateGridC <<< numBlocks, numThreads >>>(gridCounters, gridCells, pPos, pGridIndex, p);	// voxelSL, gridSL, , gridMinPosVec
	cudaThreadSynchronize();
}

//<<<<<<<<<< rigidbody kernels >>>>>>>>>>
void updateMom(float* rbMass, glm::vec3* rbForce, glm::vec3* rbPos, glm::vec3* rbLinMom, glm::vec3* rbAngMom, glm::vec3* pPos, glm::vec3* pForce){	//, float duration, float termVeloc

	//thread pro body
	int b = world->getAllBodyNum();
	int blockSize = 64;
	int numThreads = (int)fmin(blockSize, b);
	int numBlocks = nearHighVal(b, numThreads);

	updateMomC<<<numBlocks, numThreads>>>(rbMass, rbForce, rbPos, rbLinMom, rbAngMom, pPos, pForce, b);	//duration, termVeloc,
	cudaThreadSynchronize();
}

void iterate(float* rbMass, glm::vec3* rbPos, glm::vec3* rbVeloc, glm::vec3* rbLinMom, glm::quat* rbRotQuat, glm::mat3* rbRotMat, glm::vec3* rbAngVeloc, glm::vec3* rbAngMom, glm::vec3* initIITDiago, glm::mat3* inverInertTens){	//float duration, float pRadius

	//thread pro body
	int b = world->getAllBodyNum();
	int blockSize = 64;
	int numThreads = (int)fmin(blockSize, b);
	int numBlocks = nearHighVal(b, numThreads);

	iterateC<<<numBlocks, numThreads>>>(rbMass, rbPos, rbVeloc, rbLinMom, rbRotQuat, rbRotMat, rbAngVeloc, rbAngMom, initIITDiago, inverInertTens,b);	//, duration, pRadius
	cudaThreadSynchronize();
}

//<<<<<<<<<< particles kernels >>>>>>>>>>
void calcCollForces(float* pMass, glm::vec3* pPos, glm::vec3* pVeloc, glm::vec3* pForce, glm::ivec3* pGridIndex, int* gridCounters, glm::ivec4* gridCells){	//, int gridSL , float pRadius, float worldS, float springC, float dampC

	//thread pro part.
	int p = world->getAllPartNum();
		
	int blockSize = 64;
	int numThreads = (int)fmin(blockSize, p);
	int numBlocks = nearHighVal(p, numThreads);
	
	calcCollForcesC <<< numBlocks, numThreads >>>(pMass, pPos, pVeloc, pForce, pGridIndex, gridCounters, gridCells, p);	// gridSL, , pRadius, worldS, springC, dampC
	cudaThreadSynchronize();
}

void updatePart(glm::vec3* rbPos, glm::vec3* rbVeloc, glm::mat3* rbRotMat, glm::vec3* rbAngVeloc, glm::vec3* pPos, glm::vec3* pVeloc){	//, float pRadius

	//threads pro part.
	int p = world->getAllPartNum();
	int blockSize = 64;
	int numThreads = (int)fmin(blockSize, p);
	int numBlocks = nearHighVal(p, numThreads);

	updatePartC <<< numBlocks, numThreads >>>(rbPos, rbVeloc, rbRotMat, rbAngVeloc, pPos, pVeloc, p);	//pRadius,
	cudaThreadSynchronize();
}
