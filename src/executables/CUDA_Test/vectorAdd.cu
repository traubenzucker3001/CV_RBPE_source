//vectorAdd aus CUDA Samples
/*
#include <stdio.h>

// For the CUDA runtime routines (prefixed with "cuda_")
#include <cuda.h>
#include <cuda_runtime.h>
#include <device_launch_parameters.h>

#define GLM_FORCE_CUDA
#include <glm\glm.hpp>


__device__ __constant__ glm::vec3* d_A;
__device__ __constant__ glm::vec3* d_B;
__device__ __constant__ glm::vec3* d_C;

/**
 * CUDA Kernel Device code
 *
 * Computes the vector addition of A and B into C. The 3 vectors have the same
 * number of elements numElements.
 */
//__global__ void
//vectorAdd(const float *A, const float *B, float *C, int numElements)
/*__global__ void vectorAdd(glm::vec3 A, glm::vec3 B, glm::vec3 C, int numElements)
{
    int i = blockDim.x * blockIdx.x + threadIdx.x;
	if (i < 3){
		return;
	}
    //if (i < numElements){
    //    C[i] = A[i] + B[i];
    //}
	C.x = A.x + B.x;
	C.y = A.y + B.y;
	C.z = A.z + B.z;
}

/**
 * Host main routine
 */
/*int
main(void)
{
    // Error code to check return values for CUDA calls
    //cudaError_t err = cudaSuccess;

    // Print the vector length to be used, and compute its size
    int numElements = 3;
    //size_t size = numElements * sizeof(float);
	size_t size = numElements * sizeof(glm::vec3);
    printf("[Vector addition of %d elements]\n", numElements);

    // Allocate the host input vector A
    //float *h_A = (float *)malloc(size);
	glm::vec3 h_A = glm::vec3(0.0f, 0.0f, 0.0f);
    // Allocate the host input vector B
    //float *h_B = (float *)malloc(size);
	glm::vec3 h_B = glm::vec3(0.0, 0.0, 0.0);
    // Allocate the host output vector C
    //float *h_C = (float *)malloc(size);
	glm::vec3 h_C = glm::vec3(0.0, 0.0, 0.0);

    // Verify that allocations succeeded
    //if (h_A == NULL || h_B == NULL || h_C == NULL){
    //    fprintf(stderr, "Failed to allocate host vectors!\n");
    //   exit(EXIT_FAILURE);
    //}

    // Initialize the host input vectors
    //for (int i = 0; i < numElements; ++i){
        //h_A[i] = rand()/(float)RAND_MAX;
		h_A.x = rand() / (float)RAND_MAX;
		h_A.y = rand() / (float)RAND_MAX;
		h_A.z = rand() / (float)RAND_MAX;
        //h_B[i] = rand()/(float)RAND_MAX;
		h_B.x = rand() / (float)RAND_MAX;
		h_B.y = rand() / (float)RAND_MAX;
		h_B.z = rand() / (float)RAND_MAX;
   // }

    // Allocate the device input vector A
	//glm::vec3 *d_A = NULL;
    //cudaMalloc((void **)&d_A, size);
	//__device__ __constant__ glm::vec3 *d_A;
    // Allocate the device input vector B
	//glm::vec3 *d_B = NULL;
    //cudaMalloc((void **)&d_B, size);
	//__device__ __constant__ glm::vec3 *d_B;
    // Allocate the device output vector C
	//glm::vec3 *d_C = NULL;
    //cudaMalloc((void **)&d_C, size);
	//__device__ __constant__ glm::vec3 *d_C;

    // Copy the host input vectors A and B in host memory to the device input vectors in
    // device memory
    printf("Copy input data from the host memory to the CUDA device\n");
    //cudaMemcpy(d_A, h_A, size, cudaMemcpyHostToDevice);
    //cudaMemcpy(d_B, h_B, size, cudaMemcpyHostToDevice);
	cudaMemcpyToSymbol("d_A", &h_A, size); 
	cudaMemcpyToSymbol("d_B", &h_B, size);

    // Launch the Vector Add CUDA Kernel
    int threadsPerBlock = 256;
    int blocksPerGrid =(numElements + threadsPerBlock - 1) / threadsPerBlock;
    printf("CUDA kernel launch with %d blocks of %d threads\n", blocksPerGrid, threadsPerBlock);
    //vectorAdd<<<blocksPerGrid, threadsPerBlock>>>(d_A, d_B, d_C, numElements);
	vectorAdd <<<blocksPerGrid, threadsPerBlock >>>(h_A, h_B, h_C, numElements);
	cudaGetLastError();

    // Copy the device result vector in device memory to the host result vector
    // in host memory.
    printf("Copy output data from the CUDA device to the host memory\n");
	//cudaMemcpy(h_C, d_C, size, cudaMemcpyDeviceToHost);
	cudaMemcpyFromSymbol(&h_C, d_C, size);

    // Verify that the result vector is correct
    for (int i = 0; i < numElements; ++i)
    {
        //if (fabs(h_A[i] + h_B[i] - h_C[i]) > 1e-5){
        //    fprintf(stderr, "Result verification failed at element %d!\n", i);
        //    exit(EXIT_FAILURE);
        //}
		if (fabs(h_A.x + h_B.x - h_C.x) > 1e-5){
			fprintf(stderr, "Result verification failed at element %d!\n", i);
			exit(EXIT_FAILURE);
		}
		if (fabs(h_A.y + h_B.y - h_C.y) > 1e-5){
			fprintf(stderr, "Result verification failed at element %d!\n", i);
			exit(EXIT_FAILURE);
		}
		if (fabs(h_A.z + h_B.z - h_C.z) > 1e-5){
			fprintf(stderr, "Result verification failed at element %d!\n", i);
			exit(EXIT_FAILURE);
		}
    }

    printf("Test PASSED\n");

    // Free device global memory
    //cudaFree(d_A);
	//cudaFree(d_B);
    //cudaFree(d_C);

    // Free host memory
    //free(h_A);
    //free(h_B);
    //free(h_C);
	//delete h_A;
	//delete h_B;
	//delete h_C;

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
*/
