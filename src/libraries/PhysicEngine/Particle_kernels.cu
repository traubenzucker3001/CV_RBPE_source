
// <<<<<<<<<< includes >>>>>>>>>> //
#include <cuda.h>
#include <cuda_runtime.h>
#include <device_launch_parameters.h>

#include <glm\glm.hpp>

//collision detection
void calcCollForces(){

	//blocks und threads berechn.

	calcCollForcesC <<< numBlocks, numThreads >>>();
}

__global__ void calcCollForcesC(){

	//TODO
}



//update particles
//wie heiﬂt es in cpu version??
void updatePart(){

	//blocks und threads berechn.

	updatePartC <<< numBlocks, numThreads >>>();
}

__global__ void updatePartC(){

	//TODO
}