#include <cuda.h>
#include <cuda_runtime.h>
#include <device_launch_parameters.h>

#define GLM_FORCE_CUDA
#include <glm\glm.hpp>

__global__ void vectorAddC(glm::vec3* A, glm::vec3* B, glm::vec3* C, int numElements){
	int i = blockDim.x * blockIdx.x + threadIdx.x;

	if (i < numElements)
	{
		//C[i] = A[i] + B[i];
		C[i].x = A[i].x + B[i].x;
		C[i].y = A[i].y + B[i].y;
		C[i].z = A[i].z + B[i].z;
	}
}