
// <<<<<<<<<< includes >>>>>>>>>> //
#include <cuda.h>
#include <cuda_runtime.h>
#include <device_launch_parameters.h>

#include <glm\glm.hpp>

//update momenta
void updateMom(){

	//blocks und threads berechn.

	updateMomC <<< numBlocks, numThreads >>>();
}

__global__ void updateMomC(){

	//TODO
}

//perform step
void iterate(){

	//blocks und threads berechn.

	iterateC <<< numBlocks, numThreads >>>();
}

__global__ void iterateC(){

	//TODO
}