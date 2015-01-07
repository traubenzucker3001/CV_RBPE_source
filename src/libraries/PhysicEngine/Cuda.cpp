
// <<<<<<<<<< includes >>>>>>>>>> //
#ifdef WIN32
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

#include "kernels.cuh"
//#include "kernels_impl.cuh"

#include <cuda_gl_interop.h>

using namespace std;

//link fix try 4
extern Demo* demo;
extern World* world;


Cuda::Cuda(int bnIN, int pnIN){

	bodyNum = bnIN;
	partNum = pnIN;
	gridSize = 0;	// UniformGrid::getInstance()->getGridSize();	//wy 0?

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
	h_gridSL = 0;
	h_worldS = 0;
	h_springC = 0;
	h_dampC = 0;
	h_pRadius = 0;
	h_duration = 0;
	h_termVeloc = 0;

	h_gridMinPosVector = glm::vec3(0,0,0);
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

	//update VOs
	h_uVOpos = new glm::vec3[bodyNum];
	h_uVOrot = new glm::quat[bodyNum];

	//konstante vars direkt füllen
	float tempWS = world->getWorldSize();
	h_worldS = tempWS;
	h_springC = world->getSpringCoeff();
	h_dampC = world->getDampCoeff();
	h_pRadius = world->getPartRadius();
	h_duration = demo->getDuration();
	h_termVeloc = demo->getTerminalVeloc();

	//init gitter
	UniformGrid::getInstance()->createGrid();
	gridSize = UniformGrid::getInstance()->getGridSize();
	initCUDAGrid();

	float tempVS = UniformGrid::getInstance()->getVoxelSize();
	h_voxelS = tempVS;
	h_gridSL = UniformGrid::getInstance()->getGridLength();
	h_gridMinPosVector.x = -tempWS - tempVS;
	h_gridMinPosVector.y = -tempVS;
	h_gridMinPosVector.z = -tempWS - tempVS;

	//initializeDeviceBuffers();
	//init device arrays
	//speicher für arrays alokieren
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
	cudaMalloc((void**)&d_pMass, partNum*sizeof(float));
	cudaMalloc((void**)&d_pPos, partNum*sizeof(glm::vec3));
	cudaMalloc((void**)&d_pVeloc, partNum*sizeof(glm::vec3));
	cudaMalloc((void**)&d_pForce, partNum*sizeof(glm::vec3));

	//fülle host arrays
	updateHostArrays();

	//kopiere auf gpu
	hostToDevice();
}

void Cuda::updateHostArrays(){

	for (int i = 0; i < bodyNum; i++) {
		
		world->allBodies[i]->updateCUDArray(i);
	}
	for (int i=0; i<partNum; i++) {
		
		world->allParticles[i]->updateCUDArray(i);
	}
}

void Cuda::hostToDevice(){

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

	cudaMemcpy(d_pMass, h_pMass, partNum*sizeof(float), cudaMemcpyHostToDevice);
	cudaMemcpy(d_pPos, h_pPos, partNum*sizeof(glm::vec3), cudaMemcpyHostToDevice);
	cudaMemcpy(d_pVeloc, h_pVeloc, partNum*sizeof(glm::vec3), cudaMemcpyHostToDevice);
	cudaMemcpy(d_pForce, h_pForce, partNum*sizeof(glm::vec3), cudaMemcpyHostToDevice);

	cudaMemcpy(d_pGridIndex, h_pGridIndex, partNum*sizeof(glm::ivec3), cudaMemcpyHostToDevice);

	cudaMemcpy(d_gridCounters, h_gridCounters, gridSize*sizeof(int), cudaMemcpyHostToDevice);
	cudaMemcpy(d_gridCells, h_gridCells, gridSize*sizeof(glm::ivec4), cudaMemcpyHostToDevice);

	fillDeviceSymbols(h_voxelS, h_gridSL, h_worldS, h_springC, h_dampC, h_pRadius, h_duration, h_termVeloc, h_gridMinPosVector);
}

void Cuda::initCUDAGrid(){

	//host arrays
	h_pGridIndex = new glm::ivec3[partNum];

	h_gridCounters = new int[gridSize];
	h_gridCells = new glm::ivec4[gridSize];	//int4?!

	//
	for (int i = 0; i<gridSize; i++) {
		h_gridCells[i].x = -1;
		h_gridCells[i].y = -1;
		h_gridCells[i].z = -1;
		h_gridCells[i].w = -1;
	}

	for (int i = 0; i<gridSize; i++) {
		h_gridCounters[i] = 0;
	}

	//device arrays
	cudaMalloc((void**)&d_pGridIndex, partNum*sizeof(glm::ivec3));

	cudaMalloc((void**)&d_gridCounters, gridSize*sizeof(int));
	cudaMalloc((void**)&d_gridCells, gridSize*sizeof(glm::ivec4));	//int4?!
}

void Cuda::stepCUDA(){

	//schritte nacheinander aufrufen

	resetGrid(d_gridCounters, d_gridCells, gridSize);
	
	updateGrid(d_gridCounters, d_gridCells, d_pPos, d_pGridIndex);	//, d_voxelS, d_gridSL , d_gridMinPosVector
	
	calcCollForces(d_pMass, d_pPos, d_pVeloc, d_pForce, d_pGridIndex, d_gridCounters, d_gridCells);	//, d_gridSL , d_pRadius, d_worldS, d_springC, d_dampC

	updateMom(d_rbMass, d_rbForce, d_rbPos, d_rbLinMom, d_rbAngMom, d_pPos, d_pForce);	//, d_duration, d_termVeloc
	
	iterate(d_rbMass, d_rbPos, d_rbVeloc, d_rbLinMom, d_rbRotQuat, d_rbRotMat, d_rbAngVeloc, d_rbAngMom, d_rbInitInversInertTensDiago, d_rbInverseInertTens);	// d_duration, d_pRadius

	updatePart(d_rbPos, d_rbVeloc, d_rbRotMat, d_rbAngVeloc, d_pPos, d_pVeloc);	//, d_pRadius

	//VOs updaten
	//bzw. cuda opengl austausch
	// siehe --> cuda samples - 2_graphics - simpleGL
	/*cudaGLRegisterBufferObject(bufferObj);
	cudaGLMapBufferObject((void**)&devPtr,bufferObj);
	//kernel aufrufen
	cudaGraphicsUnmapResources(1, &cuda_vbo_resource1, 0);	// give access authority of vbo1 back to openGL  
	cudaGraphicsUnregisterResource(cuda_vbo_resource1);		// unregiste the resource   
	*/
	//mit opengl rendern
}

void Cuda::updateVOarrays(){

	cudaMemcpy(h_uVOpos, d_rbPos, bodyNum*sizeof(glm::vec3), cudaMemcpyDeviceToHost);
	cudaMemcpy(h_uVOrot, d_rbRotQuat, bodyNum*sizeof(glm::quat), cudaMemcpyDeviceToHost);
}

