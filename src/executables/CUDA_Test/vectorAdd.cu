//vectorAdd aus CUDA Samples

#include <stdio.h>
#include <cmath>

// For the CUDA runtime routines (prefixed with "cuda_")
#include <cuda.h>
#include <cuda_runtime.h>
#include <device_launch_parameters.h>

/**
* CUDA Kernel Device code
*
* Computes the vector addition of A and B into C. The 3 vectors have the same
* number of elements numElements.
*/
__global__ void vectorAdd(const float *A, const float *B, float *C, int numElements){
	int i = blockDim.x * blockIdx.x + threadIdx.x;

	if (i < numElements)
	{
		C[i] = A[i] + B[i];
	}
}

/**
* Host main routine
*/
int main(void){
	// Error code to check return values for CUDA calls
	//cudaError_t err = cudaSuccess;

	// Print the vector length to be used, and compute its size
	int numElements = 50000;
	size_t size = numElements * sizeof(float);
	printf("[Vector addition of %d elements]\n", numElements);

	// Allocate the host input (A,B) and output (C) vector
	float *h_A = (float *)malloc(size);
	float *h_B = (float *)malloc(size);
	float *h_C = (float *)malloc(size);

	// Verify that allocations succeeded
	if (h_A == NULL || h_B == NULL || h_C == NULL){
		fprintf(stderr, "Failed to allocate host vectors!\n");
		exit(EXIT_FAILURE);
	}

	// Initialize the host input vectors
	for (int i = 0; i < numElements; ++i){
		h_A[i] = rand() / (float)RAND_MAX;
		h_B[i] = rand() / (float)RAND_MAX;
	}

	// Allocate the device vectors
	float *d_A = NULL;
	cudaMalloc((void **)&d_A, size);
	float *d_B = NULL;
	cudaMalloc((void **)&d_B, size);
	float *d_C = NULL;
	cudaMalloc((void **)&d_C, size);

	// Copy the host input vectors A and B in host memory to the device input vectors in
	// device memory
	printf("Copy input data from the host memory to the CUDA device\n");
	cudaMemcpy(d_A, h_A, size, cudaMemcpyHostToDevice);
	cudaMemcpy(d_B, h_B, size, cudaMemcpyHostToDevice);

	// Launch the Vector Add CUDA Kernel
	int threadsPerBlock = 256;
	int blocksPerGrid = (numElements + threadsPerBlock - 1) / threadsPerBlock;
	printf("CUDA kernel launch with %d blocks of %d threads\n", blocksPerGrid, threadsPerBlock);
	vectorAdd << <blocksPerGrid, threadsPerBlock >> >(d_A, d_B, d_C, numElements);
	//cudaGetLastError();

	// Copy the device result vector in device memory to the host result vector
	// in host memory.
	printf("Copy output data from the CUDA device to the host memory\n");
	cudaMemcpy(h_C, d_C, size, cudaMemcpyDeviceToHost);

	// Verify that the result vector is correct
	for (int i = 0; i < numElements; ++i){
		if (fabs(h_A[i] + h_B[i] - h_C[i]) > 1e-5){
			fprintf(stderr, "Result verification failed at element %d!\n", i);
			exit(EXIT_FAILURE);
		}
	}

	printf("Test PASSED\n");

	// Free device global memory
	cudaFree(d_A);
	cudaFree(d_B);
	cudaFree(d_C);

	// Free host memory
	free(h_A);
	free(h_B);
	free(h_C);

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

