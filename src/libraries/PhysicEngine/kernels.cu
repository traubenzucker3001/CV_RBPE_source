//neuer cuda file zum lösen der include/compiler fehler

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

cudaError_t err = cudaSuccess;	//zum debuggen

int nearHighVal(int a, int b){
	return (a % b != 0) ? (a / b + 1) : (a / b);
}

void fillDeviceSymbols(float voxelS, int gridSL, float worldS, float springC, float dampC, float pRadius, float duration, float termVeloc, glm::vec3 gridMinPosVector){
	//siehe cuda programming guide. sollte eig ohne "" gehen
	err = cudaMemcpyToSymbol((const void*)&d_voxelS, &voxelS, sizeof(float));
	if (err != cudaSuccess){																				//zum debuggen
		fprintf(stderr, "Failed to allocate device vector A (error code %s)!\n", cudaGetErrorString(err));	//zum debuggen
		//exit(EXIT_FAILURE);
	}																										//zum debuggen
	err = cudaSuccess;																						//zum debuggen
	//cudaMemcpyToSymbol("d_gridS", &h_gridS, sizeof(int));
	err = cudaMemcpyToSymbol((const void*)&d_gridSL, &gridSL, sizeof(int));
	if (err != cudaSuccess){																				//zum debuggen
		fprintf(stderr, "Failed to allocate device vector A (error code %s)!\n", cudaGetErrorString(err));	//zum debuggen
		//exit(EXIT_FAILURE);
	}																										//zum debuggen
	err = cudaSuccess;																						//zum debuggen
	err = cudaMemcpyToSymbol((const void*)&d_worldS, &worldS, sizeof(float));
	if (err != cudaSuccess){																				//zum debuggen
		fprintf(stderr, "Failed to allocate device vector A (error code %s)!\n", cudaGetErrorString(err));	//zum debuggen
		//exit(EXIT_FAILURE);
	}																										//zum debuggen
	err = cudaSuccess;																						//zum debuggen
	err = cudaMemcpyToSymbol((const void*)&d_springC, &springC, sizeof(float));
	if (err != cudaSuccess){																				//zum debuggen
		fprintf(stderr, "Failed to allocate device vector A (error code %s)!\n", cudaGetErrorString(err));	//zum debuggen
		//exit(EXIT_FAILURE);
	}																										//zum debuggen
	err = cudaSuccess;																						//zum debuggen
	err = cudaMemcpyToSymbol((const void*)&d_dampC, &dampC, sizeof(float));
	if (err != cudaSuccess){																				//zum debuggen
		fprintf(stderr, "Failed to allocate device vector A (error code %s)!\n", cudaGetErrorString(err));	//zum debuggen
		//exit(EXIT_FAILURE);
	}																										//zum debuggen
	err = cudaSuccess;																						//zum debuggen
	err = cudaMemcpyToSymbol((const void*)&d_pRadius, &pRadius, sizeof(float));
	if (err != cudaSuccess){																				//zum debuggen
		fprintf(stderr, "Failed to allocate device vector A (error code %s)!\n", cudaGetErrorString(err));	//zum debuggen
		//exit(EXIT_FAILURE);
	}																										//zum debuggen
	err = cudaSuccess;																						//zum debuggen
	err = cudaMemcpyToSymbol((const void*)&d_duration, &duration, sizeof(float));
	if (err != cudaSuccess){																				//zum debuggen
		fprintf(stderr, "Failed to allocate device vector A (error code %s)!\n", cudaGetErrorString(err));	//zum debuggen
		//exit(EXIT_FAILURE);
	}																										//zum debuggen
	err = cudaSuccess;																						//zum debuggen
	err = cudaMemcpyToSymbol((const void*)&d_termVeloc, &termVeloc, sizeof(float));
	if (err != cudaSuccess){																				//zum debuggen
		fprintf(stderr, "Failed to allocate device vector A (error code %s)!\n", cudaGetErrorString(err));	//zum debuggen
		//exit(EXIT_FAILURE);
	}																										//zum debuggen
	err = cudaSuccess;																						//zum debuggen

	//cudaMemcpyToSymbol("d_gridMinPosVector", &h_gridMinPosVector, sizeof(glm::vec3));
	err = cudaMemcpyToSymbol((const void*)&d_gridMinPosVecX, &gridMinPosVector.x, sizeof(float));
	if (err != cudaSuccess){																				//zum debuggen
		fprintf(stderr, "Failed to allocate device vector A (error code %s)!\n", cudaGetErrorString(err));	//zum debuggen
		//exit(EXIT_FAILURE);
	}																										//zum debuggen
	err = cudaSuccess;																						//zum debuggen
	err = cudaMemcpyToSymbol((const void*)&d_gridMinPosVecY, &gridMinPosVector.y, sizeof(float));
	if (err != cudaSuccess){																				//zum debuggen
		fprintf(stderr, "Failed to allocate device vector A (error code %s)!\n", cudaGetErrorString(err));	//zum debuggen
		//exit(EXIT_FAILURE);
	}																										//zum debuggen
	err = cudaSuccess;																						//zum debuggen
	err = cudaMemcpyToSymbol((const void*)&d_gridMinPosVecZ, &gridMinPosVector.z, sizeof(float));
	if (err != cudaSuccess){																				//zum debuggen
		fprintf(stderr, "Failed to allocate device vector A (error code %s)!\n", cudaGetErrorString(err));	//zum debuggen
		//exit(EXIT_FAILURE);
	}																										//zum debuggen
	err = cudaSuccess;																						//zum debuggen
}

//extern "C"{
	//<<<<<<<<<< uniformgrid kernels >>>>>>>>>>
		void resetGrid(int* gridCounters, glm::ivec4* gridCells, int g){

		//blocks und threads berechn.
		//thread pro gitterzelle	//wie komm ich an diese zahl?!
		//int g = UniformGrid::getInstance()->getGridSize();	//gridsize=0, why?!
		cout << "gridsize: " << g << endl;	//zum debuggen
		int blockSize = 64;	//64, 256, 512, 1024
		//int numThreads = (int)fmin(blockSize, g);
		int numThreads = blockSize;
		int numBlocks = nearHighVal(g, numThreads);	
		//int numBlocks = ((g + 1024 -1) / 1024);
		cout << "threads: " << numThreads << endl;	//zum debuggen
		cout << "blocks: " << numBlocks << endl;	//zum debuggen
		//geht doch bestimmt auch noch "besser"!!?
		//int blocksPerGrid = (numElements + threadsPerBlock - 1) / threadsPerBlock;	//aus vectorAdd

		resetGridC <<< numBlocks, numThreads >>>(gridCounters, gridCells, g);
		cudaThreadSynchronize();
	}

	//updateGRid
	void updateGrid(int* gridCounters, glm::ivec4* gridCells, glm::vec3* pPos, glm::ivec3* pGridIndex){	//, float voxelSL, int gridSL , glm::vec3 gridMinPosVec

		//blocks und threads berechn.
		//int b = World::getInstance()->getAllBodyNum();	//wird bodies oder particle benötigt, oder gitter abhängiges
		//thread pro part.
		int p = world->getAllPartNum();
		//cout << "gridsize: " << p << endl;	//zum debuggen
		int blockSize = 64;
		int numThreads = (int)fmin(blockSize, p);
		int numBlocks = nearHighVal(p, numThreads);
		//cout << "threads: " << numThreads << endl;	//zum debuggen
		//cout << "blocks: " << numBlocks << endl;	//zum debuggen
		//geht doch bestimmt auch noch "besser"!!?

		updateGridC <<< numBlocks, numThreads >>>(gridCounters, gridCells, pPos, pGridIndex, p);	// voxelSL, gridSL, , gridMinPosVec
		cudaThreadSynchronize();
	}

	//<<<<<<<<<< rigidbody kernels >>>>>>>>>>
	//update momenta
	void updateMom(float* rbMass, glm::vec3* rbForce, glm::vec3* rbPos, glm::vec3* rbLinMom, glm::vec3* rbAngMom, glm::vec3* pPos, glm::vec3* pForce){	//, float duration, float termVeloc

		//todo: blocks und threads berechn.
		//thread pro body
		int b = world->getAllBodyNum();
		int blockSize = 64;
		int numThreads = (int)fmin(blockSize, b);
		int numBlocks = nearHighVal(b, numThreads);
		//geht doch bestimmt auch noch "besser"!!?

		updateMomC<<<numBlocks, numThreads>>>(rbMass, rbForce, rbPos, rbLinMom, rbAngMom, pPos, pForce, b);	//duration, termVeloc,
		cudaThreadSynchronize();
	}

	//perform step
	void iterate(float* rbMass, glm::vec3* rbPos, glm::vec3* rbVeloc, glm::vec3* rbLinMom, glm::quat* rbRotQuat, glm::mat3* rbRotMat, glm::vec3* rbAngVeloc, glm::vec3* rbAngMom, glm::vec3* initIITDiago, glm::mat3* inverInertTens){	//float duration, float pRadius

		//todo: blocks und threads berechn.
		//thread pro body
		int b = world->getAllBodyNum();
		int blockSize = 64;
		int numThreads = (int)fmin(blockSize, b);
		int numBlocks = nearHighVal(b, numThreads);
		//geht doch bestimmt auch noch "besser"!!?

		iterateC<<<numBlocks, numThreads>>>(rbMass, rbPos, rbVeloc, rbLinMom, rbRotQuat, rbRotMat, rbAngVeloc, rbAngMom, initIITDiago, inverInertTens,b);	//, duration, pRadius
		cudaThreadSynchronize();
	}

	//<<<<<<<<<< particles kernels >>>>>>>>>>
	//collision detection
	void calcCollForces(float* pMass, glm::vec3* pPos, glm::vec3* pVeloc, glm::vec3* pForce, glm::ivec3* pGridIndex, int* gridCounters, glm::ivec4* gridCells){	//, int gridSL , float pRadius, float worldS, float springC, float dampC

		//blocks und threads berechn.
		//thread pro part.
		int p = world->getAllPartNum();
		cout << "partnum: " << p << endl;	//zum debuggen
		int blockSize = 512;
		int numThreads = (int)fmin(blockSize, p);
		int numBlocks = nearHighVal(p, numThreads);
		//geht doch bestimmt auch noch "besser"!!?
		cout << "threads: " << numThreads << endl;	//zum debuggen
		cout << "blocks: " << numBlocks << endl;	//zum debuggen

		calcCollForcesC <<< numBlocks, numThreads >>>(pMass, pPos, pVeloc, pForce, pGridIndex, gridCounters, gridCells, p);	// gridSL, , pRadius, worldS, springC, dampC
		cudaThreadSynchronize();
	}

	void updatePart(glm::vec3* rbPos, glm::vec3* rbVeloc, glm::mat3* rbRotMat, glm::vec3* rbAngVeloc, glm::vec3* pPos, glm::vec3* pVeloc){	//, float pRadius

		//blocks und threads berechn.
		//threads pro part.
		int p = world->getAllPartNum();
		int blockSize = 64;
		int numThreads = (int)fmin(blockSize, p);
		int numBlocks = nearHighVal(p, numThreads);
		//geht doch bestimmt auch noch "besser"!!?

		updatePartC <<< numBlocks, numThreads >>>(rbPos, rbVeloc, rbRotMat, rbAngVeloc, pPos, pVeloc, p);	//pRadius,
		cudaThreadSynchronize();
	}

//}