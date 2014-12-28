#include <cuda.h>
#include <cuda_runtime.h>
#include <device_launch_parameters.h>

#define GLM_FORCE_CUDA
#include <glm\glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

__global__ void vectorAddC(int* A, glm::ivec4* B, int numElements){
	int i = blockDim.x * blockIdx.x + threadIdx.x;

	if (i < numElements){
		//C[i] = A[i] + B[i];
		/*C[i].w = A[i].w + B[i].w;
		C[i].x = A[i].x + B[i].x;
		C[i].y = A[i].y + B[i].y;
		C[i].z = A[i].z + B[i].z;*/
		//C[i].w = A[i].w + B[i].w;

		A[i] = 0;
		B[i].x = -1;
		B[i].y = -1;
		B[i].z = -1;
		B[i].w = -1;
	}
}