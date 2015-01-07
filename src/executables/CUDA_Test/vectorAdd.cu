//vectorAdd aus CUDA Samples

#include <stdio.h>
#include <iostream>
#include <cmath>

// For the CUDA runtime routines (prefixed with "cuda_")
#include <cuda.h>
#include <cuda_runtime.h>
#include <device_launch_parameters.h>

#define GLM_FORCE_CUDA
#include <glm\glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include "vectorAdd.cuh"
#include "vectorAdd_impl.cuh"

using namespace std;

void vectorAdd(int* A, glm::ivec4* B, int numEl){

	// Launch the Vector Add CUDA Kernel
	int threadsPerBlock = 64;	//256
	int blocksPerGrid = (numEl + threadsPerBlock - 1) / threadsPerBlock;
	printf("CUDA kernel launch with %d blocks of %d threads\n", blocksPerGrid, threadsPerBlock);
	//vectorAdd <<<blocksPerGrid, threadsPerBlock >>>(d_A, d_B, d_C, numElements);
	vectorAddC <<<blocksPerGrid, threadsPerBlock >>>(A, B, numEl);
}

void vectorAdd2(float* pMass, glm::vec3* pPos, glm::vec3* pVeloc, glm::vec3* pForce, float pRadius, float worldS, float springC, float dampC, glm::ivec3* pGridIndex, int* gridCounters, glm::ivec4* gridCells, int gridSL, int numEl){
	int threadsPerBlock = 64;	//256
	int blocksPerGrid = (numEl + threadsPerBlock - 1) / threadsPerBlock;
	printf("CUDA kernel launch with %d blocks of %d threads\n", blocksPerGrid, threadsPerBlock);

	vectorAddCU <<<blocksPerGrid, threadsPerBlock >>>(pMass, pPos, pVeloc, pForce, pRadius, worldS, springC, dampC, pGridIndex, gridCounters, gridCells, gridSL, numEl);
}
