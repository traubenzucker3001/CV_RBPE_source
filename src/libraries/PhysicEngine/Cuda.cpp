
// <<<<<<<<<< includes >>>>>>>>>> //
#include <cuda.h>
#include <cuda_runtime.h>
#include <cuda_gl_interop.h>

#include "Cuda.h"
#include "World.h"
#include "RigidBody.h"
#include "Particle.h"

#include "UniformGrid_kernels.cu"
#include "RigidBody_kernels.cu"
#include "Particle_kernels.cu"


Cuda::Cuda(int bnIN, int pnIN){

	bodyNum = bnIN;
	partNum = pnIN;

	h_rbMass = 0;
	h_rbForce = 0;
	h_rbPos = 0;
	h_rbVeloc = 0;
	h_rbLinMom = 0;
	h_rgRotQuat = 0;
	h_rbRotMat = 0;
	h_rbAngVeloc = 0;
	h_rbAngMom = 0;
	h_rbInitInversInertTensDiago = 0;	//Initial Inverse Inertia Tensor Diagonal
	h_rbInverseInertTens = 0;

	h_pMass = 0;
	h_pPos = 0;
	h_pVeloc = 0;
	h_pForce = 0;

	h_pGridIndex = 0;

	h_gCountGrid = 0;
	h_gIndexGrid = 0; //int4?!

	d_rbMass = 0;
	d_rbForce = 0;
	d_rbPos = 0;
	d_rbVeloc = 0;
	d_rbLinMom = 0;
	d_rgRotQuat = 0;
	d_rbRotMat = 0;
	d_rbAngVeloc = 0;
	d_rbAngMom = 0;
	d_rbInitInversInertTensDiago = 0;
	d_rbInverseInertTens = 0;

	d_pMass = 0;
	d_pPos = 0;
	d_pVeloc = 0;
	d_pForce = 0;

	d_pGridIndex = 0;

	d_gCountGrid = 0;
	d_gIndexGrid = 0; //int4?!
}

Cuda::~Cuda(){

	//...
}

void Cuda::initCUDA(){

	//...


	//initializeHostDataArrays();
	//init host arrays
	//body arrays
	h_rbMass = new float[bodyNum];
	h_rbForce = new glm::vec3[bodyNum];
	h_rbPos = new glm::vec3[bodyNum];
	h_rbVeloc = new glm::vec3[bodyNum];
	h_rbLinMom = new glm::vec3[bodyNum];
	h_rgRotQuat = new glm::quat[bodyNum];
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

	//initOpenCLGrid();
	//init gitter
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
	cudaMalloc((void**)&d_rgRotQuat, bodyNum*sizeof(glm::quat));
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

	//updateHostDataArrays();
	//fülle host arrays
	updateHostArrays();

	//writeHostDataToDevice();
	//kopiere auf gpu
	hostToDevice();
}

void Cuda::updateHostArrays(){

	//entscheiden ob vector o. arrays, dann entspr. anpassungen vornehmen
	RigidBody** allB = World::getInstance()->getAllBodies();
	Particle** allP = World::getInstance()->getAllParticles();
	for (int i=0; i<bodyNum; i++) {
		allB[i]->updateCUDArray(i);
	}
	for (int i=0; i<partNum; i++) {
		allP[i]->updateCUDArray(i);
	}
}

void Cuda::hostToDevice(){

	//cudaMemcpy(d_A, h_A, size, cudaMemcpyHostToDevice);
	cudaMemcpy(d_rbMass, h_rbMass, bodyNum*sizeof(float), cudaMemcpyHostToDevice);
	cudaMemcpy(d_rbForce, h_rbForce, bodyNum*sizeof(glm::vec3), cudaMemcpyHostToDevice);
	cudaMemcpy(d_rbPos, h_rbPos, bodyNum*sizeof(glm::vec3), cudaMemcpyHostToDevice);
	cudaMemcpy(d_rbVeloc, h_rbVeloc, bodyNum*sizeof(glm::vec3), cudaMemcpyHostToDevice);
	cudaMemcpy(d_rbLinMom, h_rbLinMom, bodyNum*sizeof(glm::vec3), cudaMemcpyHostToDevice);
	cudaMemcpy(d_rgRotQuat, h_rgRotQuat, bodyNum*sizeof(glm::quat), cudaMemcpyHostToDevice);
	cudaMemcpy(d_rbRotMat, h_rbRotMat, bodyNum*sizeof(glm::mat3), cudaMemcpyHostToDevice);
	cudaMemcpy(d_rbAngVeloc, h_rbAngVeloc, bodyNum*sizeof(glm::vec3), cudaMemcpyHostToDevice);
	cudaMemcpy(d_rbAngMom, h_rbAngMom, bodyNum*sizeof(glm::vec3), cudaMemcpyHostToDevice);
	cudaMemcpy(d_rbInitInversInertTensDiago, h_rbInitInversInertTensDiago, bodyNum*sizeof(glm::vec3), cudaMemcpyHostToDevice);
	cudaMemcpy(d_rbInverseInertTens, h_rbInverseInertTens, bodyNum*sizeof(glm::mat3), cudaMemcpyHostToDevice);

	cudaMemcpy(d_pMass, h_pMass, bodyNum*sizeof(float), cudaMemcpyHostToDevice);
	cudaMemcpy(d_pPos, h_pPos, bodyNum*sizeof(glm::vec3), cudaMemcpyHostToDevice);
	cudaMemcpy(d_pVeloc, h_pVeloc, bodyNum*sizeof(glm::vec3), cudaMemcpyHostToDevice);
	cudaMemcpy(d_pForce, h_pForce, bodyNum*sizeof(glm::vec3), cudaMemcpyHostToDevice);
}

void Cuda::initCUDAGrid(){

	//TODO

	//host arrays
	h_pGridIndex = new glm::vec3[partNum];

	int gS = UniformGrid::getInstance()->getGridSize();
	h_gCountGrid = new int[gS];
	h_gIndexGrid = new glm::vec4[gS];	//int4?!

	//device arrays
	cudaMalloc((void**)&d_pGridIndex, bodyNum*sizeof(glm::vec3));

	cudaMalloc((void**)&d_gCountGrid, bodyNum*sizeof(int));
	cudaMalloc((void**)&d_gIndexGrid, bodyNum*sizeof(glm::vec4));	//int4?!
}

void Cuda::stepCUDA(){

	//TODO

	//schritte nacheinander aufrufen
	//...
	resetGrid();
	updateGrid();
	calcCollForces();
	updateMom();
	iterate();
	updatePart();

	//rendern!?
	//bzw. cuda opengl austausch
	
	//irgwo vorher noch vbo mit cuda daten initial füllen, hier dann immer updaten!!
	cudaGLRegisterBufferObject(bufferObj);
	cudaGLMapBufferObject((void**)&devPtr,bufferObj);

	//kernel aufrufen

	cudaGraphicsUnmapResources(1, &cuda_vbo_resource1, 0);	// give access authority of vbo1 back to openGL  
	cudaGraphicsUnregisterResource(cuda_vbo_resource1);		// unregiste the resource   

	//mit opengl rendern

}

/*
void Cuda::initHostArrays(){

	//host arrays füllen
	//mit in init gepackt
}
*/
