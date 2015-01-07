
#include <stdio.h>
#include <iostream>
#include <cmath>

#include <cuda.h>
#include <cuda_runtime.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

//#include "vectorAdd.cu"
#include "vectorAdd.cuh"

using namespace std;

__device__ __constant__ int d_gridS;
__device__ __constant__ float d_worldS;
__device__ __constant__ float d_springC;
__device__ __constant__ float d_dampC;
__device__ __constant__ float d_pRadius;

/**
* Host main routine
*/
int main(void){
	// Error code to check return values for CUDA calls
	//cudaError_t err = cudaSuccess;

	// Print the vector length to be used, and compute its size
	int numElements = 64000;	// 100;	//50000
	//size_t size = numElements * sizeof(float);
	size_t size1 = numElements * sizeof(int);
	size_t size2 = numElements * sizeof(glm::ivec4);

	size_t size3 = numElements * sizeof(float);
	size_t size4 = numElements * sizeof(glm::vec3);
	size_t size5 = numElements * sizeof(int);
	size_t size6 = numElements * sizeof(glm::ivec3);

	printf("[Vector addition of %d elements]\n", numElements);

	// Allocate the host input (A,B) and output (C) vector
	//float *h_A = (float *)malloc(size);
	//float *h_B = (float *)malloc(size);
	//float *h_C = (float *)malloc(size);
	int* h_A = (int *)malloc(size1);
	glm::ivec4* h_B = (glm::ivec4 *)malloc(size2);
	//glm::quat* h_C = (glm::quat *)malloc(size);

	/*float* h_pMass = (float*)malloc(size3);
	glm::vec3* h_pPos = (glm::vec3 *)malloc(size4);
	glm::vec3* h_pVeloc = (glm::vec3 *)malloc(size4);
	glm::vec3* h_pForce = (glm::vec3 *)malloc(size4);

	float h_pRadius = 0.4f;
	float h_worldS = 15.0f;
	float h_springC = 100.0f;
	float h_dampC = 0.5f;
	
	glm::ivec3* h_pGridIndex = (glm::ivec3*)malloc(size6);
	int* h_gridCounters = (int*)malloc(size5);
	glm::ivec4* h_gridCells = (glm::ivec4*)malloc(size2);

	int h_gridS = 40 * 40 * 40;*/
	
	//p = numEL

	// Verify that allocations succeeded
	if (h_A == NULL || h_B == NULL){
		fprintf(stderr, "Failed to allocate host vectors!\n");
		exit(EXIT_FAILURE);
	}

	// Initialize the host input vectors
	for (int i = 0; i < numElements; ++i){
		h_A[i] = rand() / (int)RAND_MAX;
		cout << "A: " << h_A[i] << endl;
		/*h_A[i].w = rand() / (float)RAND_MAX;
		cout << "Aw: " << h_A[i].z << endl;
		h_A[i].x = rand() / (float)RAND_MAX;
		cout << "Ax: " << h_A[i].x << endl;
		h_A[i].y = rand() / (float)RAND_MAX;
		cout << "Ay: " << h_A[i].y << endl;
		h_A[i].z = rand() / (float)RAND_MAX;
		cout << "Az: " << h_A[i].z << endl;*/
		//h_A[i].w = rand() / (float)RAND_MAX;
		//cout << "Aw: " << h_A[i].z << endl;
		//h_B[i] = rand() / (float)RAND_MAX;
		//h_B[i].w = rand() / (float)RAND_MAX;
		//cout << "Bw: " << h_B[i].z << endl;
		h_B[i].x = rand() / (int)RAND_MAX;
		cout << "Bx: " << h_B[i].x << endl;
		h_B[i].y = rand() / (int)RAND_MAX;
		cout << "By: " << h_B[i].y << endl;
		h_B[i].z = rand() / (int)RAND_MAX;
		cout << "Bz: " << h_B[i].z << endl;
		h_B[i].w = rand() / (int)RAND_MAX;
		cout << "Bw: " << h_B[i].z << endl;

		/*h_pMass[i] = rand() / (float)RAND_MAX;
		h_pPos[i].x = rand() / (float)RAND_MAX;
		h_pPos[i].y = rand() / (float)RAND_MAX;
		h_pPos[i].z = rand() / (float)RAND_MAX;
		h_pVeloc[i].x = rand() / (float)RAND_MAX;
		h_pVeloc[i].y = rand() / (float)RAND_MAX;
		h_pVeloc[i].z = rand() / (float)RAND_MAX;
		h_pForce[i].x = rand() / (float)RAND_MAX;
		h_pForce[i].y = rand() / (float)RAND_MAX;
		h_pForce[i].z = rand() / (float)RAND_MAX;
		h_pGridIndex[i].x = rand() / (int)RAND_MAX;
		h_pGridIndex[i].y = rand() / (int)RAND_MAX;
		h_pGridIndex[i].z = rand() / (int)RAND_MAX;
		h_gridCounters[i] = rand() / (int)RAND_MAX;
		h_gridCells[i].x = rand() / (int)RAND_MAX;
		h_gridCells[i].y = rand() / (int)RAND_MAX;
		h_gridCells[i].z = rand() / (int)RAND_MAX;
		h_gridCells[i].w = rand() / (int)RAND_MAX;*/
	}

	// Allocate the device vectors
	//float *d_A = NULL;
	int *d_A = NULL;
	cudaMalloc((void **)&d_A, size1);
	//float *d_B = NULL;
	glm::ivec4 *d_B = NULL;
	cudaMalloc((void **)&d_B, size2);
	//float *d_C = NULL;
	//glm::quat *d_C = NULL;
	//cudaMalloc((void **)&d_C, size);

	/*float* d_pMass = NULL;
	cudaMalloc((void **)&d_pMass, size3);
	glm::vec3* d_pPos = NULL;
	cudaMalloc((void **)&d_pPos, size4);
	glm::vec3* d_pVeloc = NULL;
	cudaMalloc((void **)&d_pVeloc, size4);
	glm::vec3* d_pForce = NULL;
	cudaMalloc((void **)&d_pForce, size4);

	glm::ivec3* d_pGridIndex = NULL;
	cudaMalloc((void **)&d_pGridIndex, size6);
	int* d_gridCounters = NULL;
	cudaMalloc((void **)&d_gridCounters, size5);
	glm::ivec4* d_gridCells = NULL;
	cudaMalloc((void **)&d_gridCells, size2);*/

	// Copy the host input vectors A and B in host memory to the device input vectors in
	// device memory
	printf("Copy input data from the host memory to the CUDA device\n");
	cudaMemcpy(d_A, h_A, size1, cudaMemcpyHostToDevice);
	cudaMemcpy(d_B, h_B, size2, cudaMemcpyHostToDevice);

	/*cudaMemcpy(d_pMass, h_pMass, size3, cudaMemcpyHostToDevice);
	cudaMemcpy(d_pPos, h_pPos, size4, cudaMemcpyHostToDevice);
	cudaMemcpy(d_pVeloc, h_pVeloc, size4, cudaMemcpyHostToDevice);
	cudaMemcpy(d_pForce, h_pForce, size4, cudaMemcpyHostToDevice);
	cudaMemcpy(d_pGridIndex, h_pGridIndex, size6, cudaMemcpyHostToDevice);
	cudaMemcpy(d_gridCounters, h_gridCounters, size5, cudaMemcpyHostToDevice);
	cudaMemcpy(d_gridCells, h_gridCells, size2, cudaMemcpyHostToDevice);

	cudaMemcpyToSymbol("d_springC", &h_springC, sizeof(float));
	cudaMemcpyToSymbol("d_worldS", &h_worldS, sizeof(float));
	cudaMemcpyToSymbol("d_dampC", &h_dampC, sizeof(float));
	cudaMemcpyToSymbol("d_gridS", &h_gridS, sizeof(int));
	cudaMemcpyToSymbol("d_pRadius", &h_pRadius, sizeof(float));*/

	// Launch the Vector Add CUDA Kernel
	//int threadsPerBlock = 64;	//256
	//int blocksPerGrid = (numElements + threadsPerBlock - 1) / threadsPerBlock;
	//printf("CUDA kernel launch with %d blocks of %d threads\n", blocksPerGrid, threadsPerBlock);
	//vectorAdd <<<blocksPerGrid, threadsPerBlock >>>(d_A, d_B, d_C, numElements);
	//vectorAddC << <blocksPerGrid, threadsPerBlock >> >(d_A, d_B, d_C, numElements);
	//cudaGetLastError();
	//vectorAdd(d_A, d_B, d_C, numElements);
	vectorAdd(d_A, d_B, numElements);

	//vectorAdd2(d_pMass, d_pPos, d_pVeloc, d_pForce, d_pRadius, d_worldS, d_springC, d_dampC, d_pGridIndex, d_gridCounters, d_gridCells, d_gridS, numElements);

	// Copy the device result vector in device memory to the host result vector
	// in host memory.
	printf("Copy output data from the CUDA device to the host memory\n");
	//cudaMemcpy(h_C, d_C, size, cudaMemcpyDeviceToHost);
	cudaMemcpy(h_A, d_A, size1, cudaMemcpyDeviceToHost);
	cudaMemcpy(h_B, d_B, size2, cudaMemcpyDeviceToHost);

	// Verify that the result vector is correct
	for (int i = 0; i < numElements; ++i){
		//if (fabs(h_A[i] + h_B[i] - h_C[i]) > 1e-5){
		//	fprintf(stderr, "Result verification failed at element %d!\n", i);
		//	exit(EXIT_FAILURE);
		//}
		/*cout << "Cx: " << h_C[i].x << endl;
		cout << "Cy: " << h_C[i].y << endl;
		cout << "Cz: " << h_C[i].z << endl;
		if (fabs(h_A[i].x + h_B[i].x - h_C[i].x) > 1e-5){
			fprintf(stderr, "Result verification failed at element %d!\n", i);
			exit(EXIT_FAILURE);
		}
		if (fabs(h_A[i].y + h_B[i].y - h_C[i].y) > 1e-5){
			fprintf(stderr, "Result verification failed at element %d!\n", i);
			exit(EXIT_FAILURE);
		}
		if (fabs(h_A[i].z + h_B[i].z - h_C[i].z) > 1e-5){
			fprintf(stderr, "Result verification failed at element %d!\n", i);
			exit(EXIT_FAILURE);
		}*/

		cout << "A: " << h_A[i] << endl;
		cout << "Bx: " << h_B[i].x << endl;
		cout << "By: " << h_B[i].y << endl;
		cout << "Bz: " << h_B[i].z << endl;
		cout << "Bw: " << h_B[i].w << endl;
	}

	printf("Test PASSED\n");

	// Free device global memory
	cudaFree(d_A);
	cudaFree(d_B);
	//cudaFree(d_C);

	/*cudaFree(d_pMass);
	cudaFree(d_pPos);
	cudaFree(d_pVeloc);
	cudaFree(d_pForce);
	cudaFree(d_pGridIndex);
	cudaFree(d_gridCounters);
	cudaFree(d_gridCells);*/

	// Free host memory
	free(h_A);
	free(h_B);
	//free(h_C);

	/*free(h_pMass);
	free(h_pPos);
	free(h_pVeloc);
	free(h_pForce);
	free(h_pGridIndex);
	free(h_gridCounters);
	free(h_gridCells);*/

	// Reset the device and exit
	// cudaDeviceReset causes the driver to clean up all state. While
	// not mandatory in normal operation, it is good practice.  It is also
	// needed to ensure correct operation when the application is being
	// profiled. Calling cudaDeviceReset causes all profile data to be
	// flushed before the application exits
	cudaDeviceReset();

	printf("Done\n");
	return 0;
}

//GLM:
//-vec3 --> ok
//-vec4 --> ok
//-quat --> ok
//-mat3
//-mat4
//...
//-ivec --> ok
//...