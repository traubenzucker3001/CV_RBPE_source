/*
#include <iostream>
//#include "PhysicEngine/EarlyOut.h"

using namespace std;

#define SIZE 1024

int main() {

	int *a,*b,*c;

	a = (int *)malloc(SIZE*sizeof(int));
	b = (int *)malloc(SIZE*sizeof(int));
	c = (int *)malloc(SIZE*sizeof(int));

	for(int i=0; i<SIZE; ++i){

		a[i] = i;
		b[i] = i;
		c[i] = 0;
	}

	//funktion wieder entfernt
	//VecAddCPU(a,b,c,SIZE);

	for(int i=0; i<10; ++i){

		printf("c[%d] = %d\n",i,c[i]);
	}

	free(a);
	free(b);
	free(c);

	return 0;
}
*/


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

/**
* Host main routine
*/
int main(void){
	// Error code to check return values for CUDA calls
	//cudaError_t err = cudaSuccess;

	// Print the vector length to be used, and compute its size
	int numElements = 50;	//50000
	//size_t size = numElements * sizeof(float);
	size_t size1 = numElements * sizeof(int);
	size_t size2 = numElements * sizeof(glm::ivec4);
	printf("[Vector addition of %d elements]\n", numElements);

	// Allocate the host input (A,B) and output (C) vector
	//float *h_A = (float *)malloc(size);
	//float *h_B = (float *)malloc(size);
	//float *h_C = (float *)malloc(size);
	int* h_A = (int *)malloc(size1);
	glm::ivec4* h_B = (glm::ivec4 *)malloc(size2);
	//glm::quat* h_C = (glm::quat *)malloc(size);

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

	// Copy the host input vectors A and B in host memory to the device input vectors in
	// device memory
	printf("Copy input data from the host memory to the CUDA device\n");
	cudaMemcpy(d_A, h_A, size1, cudaMemcpyHostToDevice);
	cudaMemcpy(d_B, h_B, size2, cudaMemcpyHostToDevice);

	// Launch the Vector Add CUDA Kernel
	//int threadsPerBlock = 64;	//256
	//int blocksPerGrid = (numElements + threadsPerBlock - 1) / threadsPerBlock;
	//printf("CUDA kernel launch with %d blocks of %d threads\n", blocksPerGrid, threadsPerBlock);
	//vectorAdd <<<blocksPerGrid, threadsPerBlock >>>(d_A, d_B, d_C, numElements);
	//vectorAddC << <blocksPerGrid, threadsPerBlock >> >(d_A, d_B, d_C, numElements);
	//cudaGetLastError();
	//vectorAdd(d_A, d_B, d_C, numElements);
	vectorAdd(d_A, d_B, numElements);

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

	// Free host memory
	free(h_A);
	free(h_B);
	//free(h_C);

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