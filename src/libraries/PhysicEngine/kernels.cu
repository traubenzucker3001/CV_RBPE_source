//neuer cuda file zum lösen der include/compiler fehler

#include <cuda.h>
#include <cuda_runtime.h>

#include "UniformGrid.h"
#include "World.h"

#include "kernels_impl.cuh"

//link fix try 4
extern World* world;

int nearHighVal(int a, int b){
	return (a % b != 0) ? (a / b + 1) : (a / b);
}	//nicht in jedem cu eine funktion, irgwo für alle erreichbar machen

//extern "C"{
	//<<<<<<<<<< uniformgrid kernels >>>>>>>>>>
	void resetGrid(int* countGrid, glm::vec4* indexGrid){
		cout << "-test reset grid 1-" << endl; //zum debuggen
		//blocks und threads berechn.
		//int b = World::getInstance()->getAllBodyNum();	//wird bodies oder particle benötigt, oder gitter abhängiges
		//thread pro gitterzelle	//wie komm ich an diese zahl?!
		int g = UniformGrid::getInstance()->getGridSize();	//gridsize=0, why?!
		cout << "-test reset grid 1.3- g: " << g << endl; //zum debuggen
		int blockSize = 64;
		cout << "-test reset grid 1.5-" << endl; //zum debuggen
		int numThreads = (int)fmin(blockSize, g);
		cout << "-test reset grid 1.7- threads: " << numThreads << endl; //zum debuggen
		int numBlocks = nearHighVal(g, numThreads);		//<--- fehler dort
		//geht doch bestimmt auch noch "besser"!!?
		cout << "-test reset grid 2-" << endl; //zum debuggen
		resetGridC <<< numBlocks, numThreads >>>(countGrid, indexGrid);
		cout << "-test reset grid 3-" << endl; //zum debuggen
	}

	//updateGRid
	void updateGrid(int* countGrid, glm::vec4* indexGrid, glm::vec3* pPos, glm::vec3 gridMinPosVec, float voxelSL, int gridSL, glm::vec3* pGridIndex){

		//blocks und threads berechn.
		//int b = World::getInstance()->getAllBodyNum();	//wird bodies oder particle benötigt, oder gitter abhängiges
		//thread pro part.
		int p = world->getAllPartNum();
		int blockSize = 64;
		int numThreads = (int)fmin(blockSize, p);
		int numBlocks = nearHighVal(p, numThreads);
		//geht doch bestimmt auch noch "besser"!!?

		updateGridC <<< numBlocks, numThreads >>>(countGrid, indexGrid, pPos, gridMinPosVec, voxelSL, gridSL, pGridIndex);
	}

	//<<<<<<<<<< rigidbody kernels >>>>>>>>>>
	//update momenta
	void updateMom(float* rbMass, glm::vec3* rbForce, glm::vec3* rbPos, glm::vec3* rbLinMom, glm::vec3* rbAngMom, glm::vec3* pPos, glm::vec3* pForce, float duration, float termVeloc){

		//todo: blocks und threads berechn.
		//thread pro body
		int b = world->getAllBodyNum();
		int blockSize = 64;
		int numThreads = (int)fmin(blockSize, b);
		int numBlocks = nearHighVal(b, numThreads);
		//geht doch bestimmt auch noch "besser"!!?

		updateMomC <<< numBlocks, numThreads >>>(rbMass, rbForce, rbPos, rbLinMom, rbAngMom, pPos, pForce, duration, termVeloc);
	}

	//perform step
	void iterate(float* rbMass, glm::vec3* rbPos, glm::vec3* rbVeloc, glm::vec3* rbLinMom, glm::quat* rbRotQuat, glm::mat3* rbRotMat, glm::vec3* rbAngVeloc, glm::vec3* rbAngMom, glm::vec3* initIITDiago, glm::mat3* inverInertTens, float duration, float pRadius){

		//todo: blocks und threads berechn.
		//thread pro body
		int b = world->getAllBodyNum();
		int blockSize = 64;
		int numThreads = (int)fmin(blockSize, b);
		int numBlocks = nearHighVal(b, numThreads);
		//geht doch bestimmt auch noch "besser"!!?

		iterateC <<< numBlocks, numThreads >>>(rbMass, rbPos, rbVeloc, rbLinMom, rbRotQuat, rbRotMat, rbAngVeloc, rbAngMom, initIITDiago, inverInertTens, duration, pRadius);
	}

	//<<<<<<<<<< particles kernels >>>>>>>>>>
	//collision detection
	void calcCollForces(float* pMass, glm::vec3* pPos, glm::vec3* pVeloc, glm::vec3* pForce, float pRadius, float worldS, float springC, float dampC, glm::vec3* pGridIndex, int* countGrid, glm::vec4* indexGrid, int gridSL){

		//blocks und threads berechn.
		//thread pro part.
		int p = world->getAllPartNum();
		int blockSize = 64;
		int numThreads = (int)fmin(blockSize, p);
		int numBlocks = nearHighVal(p, numThreads);
		//geht doch bestimmt auch noch "besser"!!?

		calcCollForcesC <<< numBlocks, numThreads >>>(pMass, pPos, pVeloc, pForce, pRadius, worldS, springC, dampC, pGridIndex, countGrid, indexGrid, gridSL);
	}

	void updatePart(glm::vec3* rbPos, glm::vec3* rbVeloc, glm::mat3* rbRotMat, glm::vec3* rbAngVeloc, glm::vec3* pPos, glm::vec3* pVeloc, float pRadius){

		//blocks und threads berechn.
		//threads pro part.
		int p = world->getAllPartNum();
		int blockSize = 64;
		int numThreads = (int)fmin(blockSize, p);
		int numBlocks = nearHighVal(p, numThreads);
		//geht doch bestimmt auch noch "besser"!!?

		updatePartC <<< numBlocks, numThreads >>>(rbPos, rbVeloc, rbRotMat, rbAngVeloc, pPos, pVeloc, pRadius);
	}

//}