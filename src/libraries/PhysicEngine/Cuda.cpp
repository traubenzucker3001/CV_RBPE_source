
// <<<<<<<<<< includes >>>>>>>>>> //
#ifdef WIN32			//für behebung der gl.h errors
#include <windows.h>
#endif

#include <cuda.h>
#include <cuda_runtime.h>
#include <iostream>

#include "DemoApp\Demo.h"
#include "Cuda.h"
#include "World.h"
#include "RigidBody.h"
#include "Particle.h"
#include "UniformGrid.h"

//#include "UniformGrid_kernels.cu"	//!!!diese hier verursachen wahrscheinlich die syntax- u. atomic-fehler!!!
//#include "RigidBody_kernels.cu"		//durch andere struktur (.cuh,...) oder compiler lösen
//#include "Particle_kernels.cu"
#include "kernels.cuh"

#include <cuda_gl_interop.h>

using namespace std;

//link fix try 4
extern Demo* demo;
extern World* world;

//wollten innerhalb der klasse nicht
__device__ __constant__ float d_voxelS;
__device__ __constant__ int d_gridS;
__device__ __constant__ float d_worldS;
__device__ __constant__ float d_springC;
__device__ __constant__ float d_dampC;
__device__ __constant__ float d_pRadius;
__device__ __constant__ float d_duration;
__device__ __constant__ float d_termVeloc;

__device__ __constant__ glm::vec3 d_gridMinPosVector;

Cuda::Cuda(int bnIN, int pnIN){

	//cout << "cuda: cuda constr called!" << endl; //zum test

	bodyNum = bnIN;
	partNum = pnIN;

	h_rbMass = 0;
	h_rbForce = 0;
	h_rbPos = 0;
	h_rbVeloc = 0;
	h_rbLinMom = 0;
	h_rbRotQuat = 0;
	h_rbRotMat = 0;
	h_rbAngVeloc = 0;
	h_rbAngMom = 0;
	h_rbInitInversInertTensDiago = 0;	//Initial Inverse Inertia Tensor Diagonal
	h_rbInverseInertTens = 0;

	h_pMass = 0;
	h_pPos = 0;
	h_pVeloc = 0;
	h_pForce = 0;

	h_uVOpos = 0;
	h_uVOrot = 0;

	h_pGridIndex = 0;

	h_gridCounters = 0;
	h_gridCells = 0;

	d_rbMass = NULL;
	d_rbForce = NULL;
	d_rbPos = NULL;
	d_rbVeloc = NULL;
	d_rbLinMom = NULL;
	d_rbRotQuat = NULL;
	d_rbRotMat = NULL;
	d_rbAngVeloc = NULL;
	d_rbAngMom = NULL;
	d_rbInitInversInertTensDiago = NULL;
	d_rbInverseInertTens = NULL;

	d_pMass = NULL;
	d_pPos = NULL;
	d_pVeloc = NULL;
	d_pForce = NULL;

	d_pGridIndex = NULL;

	d_gridCounters = NULL;
	d_gridCells = NULL;

	h_voxelS = 0;
	h_gridS = 0;
	h_worldS = 0;
	h_springC = 0;
	h_dampC = 0;
	h_pRadius = 0;
	h_duration = 0;
	h_termVeloc = 0;

	h_gridMinPosVector = glm::vec3(0,0,0);

	/*
	d_voxelS = 0;
	d_gridS = 0;
	d_worldS = 0;
	d_springC = 0;
	d_dampC = 0;
	d_pRadius = 0;
	d_duration = 0;
	d_termVeloc = 0;

	d_gridMinPosVector = 0;
	*/
}

Cuda::~Cuda(){

	delete h_rbRotQuat;
	delete h_rbVeloc;
	delete h_rbRotMat;
	delete h_rbPos;
	delete h_rbMass;
	delete h_rbLinMom;
	delete h_rbInverseInertTens;
	delete h_rbInitInversInertTensDiago;
	delete h_rbForce;
	delete h_rbAngVeloc;
	delete h_rbAngMom;
	delete h_pVeloc;
	delete h_pPos;
	delete h_pMass;
	delete h_pGridIndex;
	delete h_pForce;
	delete h_gridCells;
	delete h_gridCounters;

	delete h_uVOpos;
	delete h_uVOrot;

	cudaFree(d_rbMass);
	cudaFree(d_rbForce);
	cudaFree(d_rbPos);
	cudaFree(d_rbVeloc);
	cudaFree(d_rbLinMom);
	cudaFree(d_rbRotQuat);
	cudaFree(d_rbRotMat);
	cudaFree(d_rbAngVeloc);
	cudaFree(d_rbAngMom);
	cudaFree(d_rbInitInversInertTensDiago);
	cudaFree(d_rbInverseInertTens);
	cudaFree(d_pMass);
	cudaFree(d_pPos);
	cudaFree(d_pVeloc);
	cudaFree(d_pForce);
	cudaFree(d_pGridIndex);
	cudaFree(d_gridCounters);
	cudaFree(d_gridCells);

}

void Cuda::initCUDA(){

	//cout << "cuda: initCuda called!" << endl; //zum test

	//...


	//initializeHostDataArrays();
	//init host arrays
	//body arrays
	h_rbMass = new float[bodyNum];
	h_rbForce = new glm::vec3[bodyNum];
	h_rbPos = new glm::vec3[bodyNum];
	h_rbVeloc = new glm::vec3[bodyNum];
	h_rbLinMom = new glm::vec3[bodyNum];
	h_rbRotQuat = new glm::quat[bodyNum];
	h_rbRotMat = new glm::mat3[bodyNum];
	h_rbAngVeloc = new glm::vec3[bodyNum];
	h_rbAngMom = new glm::vec3[bodyNum];
	h_rbInitInversInertTensDiago = new glm::vec3[bodyNum];
	h_rbInverseInertTens = new glm::mat3[bodyNum];

	//paticle arrays
	h_pMass = new float[partNum];
	h_pPos = new glm::vec3[partNum];
	h_pVeloc = new glm::vec3[partNum];
	h_pForce = new glm::vec3[partNum];

	//grid arrays
	//in initCUDAGrid

	//update VOs
	h_uVOpos = new glm::vec3[bodyNum];
	h_uVOrot = new glm::quat[bodyNum];

	//konstante vars direkt füllen
	float tempVS = UniformGrid::getInstance()->getVoxelSize();
	h_voxelS = tempVS;
	h_gridS = UniformGrid::getInstance()->getGridSize();
	float tempWS = world->getWorldSize();
	h_worldS = tempWS;
	h_springC = world->getSpringCoeff();
	h_dampC = world->getDampCoeff();
	h_pRadius = world->getPartRadius();
	h_duration = demo->getDuration();
	h_termVeloc = demo->getTerminalVeloc();

	h_gridMinPosVector.x = -tempWS - tempVS;
	h_gridMinPosVector.y = -tempVS;
	h_gridMinPosVector.z = -tempWS - tempVS;

	//initOpenCLGrid();
	//init gitter
	UniformGrid::getInstance()->createGrid();
	initCUDAGrid();

	//initializeDeviceBuffers();
	//init device arrays
	//speicher für arrays alokieren
	//checkCudaErrors(cudaMalloc(devPtr, size));
	cudaMalloc((void**)&d_rbMass, bodyNum*sizeof(float));
	cudaMalloc((void**)&d_rbForce, bodyNum*sizeof(glm::vec3));
	cudaMalloc((void**)&d_rbPos, bodyNum*sizeof(glm::vec3));
	cudaMalloc((void**)&d_rbVeloc, bodyNum*sizeof(glm::vec3));
	cudaMalloc((void**)&d_rbLinMom, bodyNum*sizeof(glm::vec3));
	cudaMalloc((void**)&d_rbRotQuat, bodyNum*sizeof(glm::quat));
	cudaMalloc((void**)&d_rbRotMat, bodyNum*sizeof(glm::mat3));
	cudaMalloc((void**)&d_rbAngVeloc, bodyNum*sizeof(glm::vec3));
	cudaMalloc((void**)&d_rbAngMom, bodyNum*sizeof(glm::vec3));
	cudaMalloc((void**)&d_rbInitInversInertTensDiago, bodyNum*sizeof(glm::vec3));
	cudaMalloc((void**)&d_rbInverseInertTens, bodyNum*sizeof(glm::mat3));

	//particle parameter
	cudaMalloc((void**)&d_pMass, bodyNum*sizeof(float));
	cudaMalloc((void**)&d_pPos, bodyNum*sizeof(glm::vec3));
	cudaMalloc((void**)&d_pVeloc, bodyNum*sizeof(glm::vec3));
	cudaMalloc((void**)&d_pForce, bodyNum*sizeof(glm::vec3));

	/*
	cudaMalloc((void**)d_voxelS, sizeof(float));	//todo: für diese vars speicher alloc
	cudaMalloc((void**)&d_gridS, sizeof(int));
	cudaMalloc((void**)&d_worldS, sizeof(float));
	cudaMalloc((void**)&d_springC, sizeof(float));
	cudaMalloc((void**)&d_dampC, sizeof(float));
	cudaMalloc((void**)&d_pRadius, sizeof(float));
	cudaMalloc((void**)&d_duration, sizeof(float));
	cudaMalloc((void**)&d_termVeloc, sizeof(float));

	cudaMalloc((void**)&d_gridMinPosVector, sizeof(glm::vec3));
	*/

	//cudaMemcpyToSymbol(d_voxelS,&h_voxelS,1 * sizeof(int));

	//updateHostDataArrays();
	//fülle host arrays
	updateHostArrays();

	//writeHostDataToDevice();
	//kopiere auf gpu
	hostToDevice();
}

void Cuda::updateHostArrays(){

	//cout << "cuda: updateHostArr called!" << endl; //zum test

	//RigidBody** allB = World::getInstance()->getAllBodies();
	//Particle** allP = World::getInstance()->getAllParticles();
	for (int i = 0; i < bodyNum; i++) {
		//allB[i]->updateCUDArray(i);
		world->allBodies[i]->updateCUDArray(i);
	}
	for (int i=0; i<partNum; i++) {
		//allP[i]->updateCUDArray(i);
		world->allParticles[i]->updateCUDArray(i);
	}
}

void Cuda::hostToDevice(){

	//cout << "cuda: hostTOdevice called!" << endl; //zum test

	//cudaMemcpy(d_A, h_A, size, cudaMemcpyHostToDevice);
	cudaMemcpy(d_rbMass, h_rbMass, bodyNum*sizeof(float), cudaMemcpyHostToDevice);
	cudaMemcpy(d_rbForce, h_rbForce, bodyNum*sizeof(glm::vec3), cudaMemcpyHostToDevice);
	cudaMemcpy(d_rbPos, h_rbPos, bodyNum*sizeof(glm::vec3), cudaMemcpyHostToDevice);
	cudaMemcpy(d_rbVeloc, h_rbVeloc, bodyNum*sizeof(glm::vec3), cudaMemcpyHostToDevice);
	cudaMemcpy(d_rbLinMom, h_rbLinMom, bodyNum*sizeof(glm::vec3), cudaMemcpyHostToDevice);
	cudaMemcpy(d_rbRotQuat, h_rbRotQuat, bodyNum*sizeof(glm::quat), cudaMemcpyHostToDevice);
	cudaMemcpy(d_rbRotMat, h_rbRotMat, bodyNum*sizeof(glm::mat3), cudaMemcpyHostToDevice);
	cudaMemcpy(d_rbAngVeloc, h_rbAngVeloc, bodyNum*sizeof(glm::vec3), cudaMemcpyHostToDevice);
	cudaMemcpy(d_rbAngMom, h_rbAngMom, bodyNum*sizeof(glm::vec3), cudaMemcpyHostToDevice);
	cudaMemcpy(d_rbInitInversInertTensDiago, h_rbInitInversInertTensDiago, bodyNum*sizeof(glm::vec3), cudaMemcpyHostToDevice);
	cudaMemcpy(d_rbInverseInertTens, h_rbInverseInertTens, bodyNum*sizeof(glm::mat3), cudaMemcpyHostToDevice);

	cudaMemcpy(d_pMass, h_pMass, bodyNum*sizeof(float), cudaMemcpyHostToDevice);
	cudaMemcpy(d_pPos, h_pPos, bodyNum*sizeof(glm::vec3), cudaMemcpyHostToDevice);
	cudaMemcpy(d_pVeloc, h_pVeloc, bodyNum*sizeof(glm::vec3), cudaMemcpyHostToDevice);
	cudaMemcpy(d_pForce, h_pForce, bodyNum*sizeof(glm::vec3), cudaMemcpyHostToDevice);

	cudaMemcpy(d_gridCounters, h_gridCounters, bodyNum*sizeof(int), cudaMemcpyHostToDevice);
	cudaMemcpy(d_gridCells, h_gridCells, bodyNum*sizeof(glm::ivec4), cudaMemcpyHostToDevice);

	/*
	cudaMemcpy(d_voxelS, h_voxelS, sizeof(float), cudaMemcpyHostToDevice);
	d_gridS = 0;
	d_worldS = 0;
	...
	*/
	 //siehe cuda programming guide. sollte eig ohne "" gehen
	cudaMemcpyToSymbol("d_voxelS", &h_voxelS, sizeof(float));
	cudaMemcpyToSymbol("d_gridS", &h_gridS, sizeof(int));
	cudaMemcpyToSymbol("d_worldS", &h_worldS, sizeof(float));
	cudaMemcpyToSymbol("d_springC", &h_springC, sizeof(float));
	cudaMemcpyToSymbol("d_dampC", &h_dampC, sizeof(float));
	cudaMemcpyToSymbol("d_pRadius", &h_pRadius, sizeof(float));
	cudaMemcpyToSymbol("d_duration", &h_duration, sizeof(float));
	cudaMemcpyToSymbol("d_termVeloc", &h_termVeloc, sizeof(float));

	cudaMemcpyToSymbol("d_gridMinPosVector", &h_gridMinPosVector, sizeof(glm::vec3));

	//vbo data

}

void Cuda::initCUDAGrid(){

	//cout << "cuda: initGrid called!" << endl; //zum test

	//host arrays
	h_pGridIndex = new glm::ivec3[partNum];

	int gS = UniformGrid::getInstance()->getGridSize();
	h_gridCounters = new int[gS];
	h_gridCells = new glm::ivec4[gS];	//int4?!

	//
	for (int i = 0; i<gS; i++) {
		h_gridCells[i].x = -1;
		h_gridCells[i].y = -1;
		h_gridCells[i].z = -1;
		h_gridCells[i].w = -1;
	}

	for (int i = 0; i<gS; i++) {
		h_gridCounters[i] = 0;
	}

	//device arrays
	cudaMalloc((void**)&d_pGridIndex, bodyNum*sizeof(glm::ivec3));

	cudaMalloc((void**)&d_gridCounters, bodyNum*sizeof(int));
	cudaMalloc((void**)&d_gridCells, bodyNum*sizeof(glm::ivec4));	//int4?!
}

void Cuda::stepCUDA(){

	//cout << "cuda: stepCUDA!" << endl; //zum test

	//schritte nacheinander aufrufen
	cout << "-test stepCUDA 1-" << endl; //zum debuggen
	int g = UniformGrid::getInstance()->getGridSize();
	resetGrid(d_gridCounters, d_gridCells,g);		//versuch: g hier nicht erst in.cu
	/*cout << "-test stepCUDA 2-" << endl; //zum debuggen
	updateGrid(d_gridCounters, d_gridCells, d_pPos, d_gridMinPosVector, d_voxelS, d_gridS, d_pGridIndex);
	cout << "-test stepCUDA 3-" << endl; //zum debuggen
	calcCollForces(d_pMass, d_pPos, d_pVeloc, d_pForce, d_pRadius, d_worldS, d_springC, d_dampC, d_pGridIndex, d_gridCounters, d_gridCells, d_gridS);
	cout << "-test stepCUDA 4-" << endl; //zum debuggen
	updateMom(d_rbMass, d_rbForce, d_rbPos, d_rbLinMom, d_rbAngMom, d_pPos, d_pForce, d_duration, d_termVeloc);
	cout << "-test stepCUDA 5-" << endl; //zum debuggen
	iterate(d_rbMass, d_rbPos, d_rbVeloc, d_rbLinMom, d_rbRotQuat, d_rbRotMat, d_rbAngVeloc, d_rbAngMom, d_rbInitInversInertTensDiago, d_rbInverseInertTens, d_duration, d_pRadius);
	cudaMemcpy(h_rbPos, d_rbPos, bodyNum*sizeof(glm::vec3), cudaMemcpyDeviceToHost);
	cout << "pos: " << h_rbPos[2].y << endl;	//zum debuggen
	cout << "-test stepCUDA 6-" << endl; //zum debuggen
	updatePart(d_rbPos, d_rbVeloc, d_rbRotMat, d_rbAngVeloc, d_pPos, d_pVeloc, d_pRadius);
	cout << "-test stepCUDA 7-" << endl; //zum debuggen	*/
	//VOs updaten
	//TODO

	//rendern!?
	//bzw. cuda opengl austausch
	// siehe --> cuda samples - 2_graphics - simpleGL
	
	//v1 über cvk cpu
	//TODO

	//v2 über cuda opengl (instancing)
	//irgwo vorher noch vbo mit cuda daten initial füllen, hier dann immer updaten!!
	/*
	cudaGLRegisterBufferObject(bufferObj);
	cudaGLMapBufferObject((void**)&devPtr,bufferObj);

	//kernel aufrufen

	cudaGraphicsUnmapResources(1, &cuda_vbo_resource1, 0);	// give access authority of vbo1 back to openGL  
	cudaGraphicsUnregisterResource(cuda_vbo_resource1);		// unregiste the resource   
	*/

	//mit opengl rendern

}

void Cuda::updateVOarrays(){

	cout << "update vo arrays!" << endl;	//zum debuggen

	cudaMemcpy(h_uVOpos, d_rbPos, bodyNum*sizeof(glm::vec3), cudaMemcpyDeviceToHost);
	cudaMemcpy(h_uVOrot, d_rbRotQuat, bodyNum*sizeof(glm::quat), cudaMemcpyDeviceToHost);
}

