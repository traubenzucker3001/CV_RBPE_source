/*
#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "ContactGen.h"
#include "EarlyOut.h"
#include "Core.h"
#include <cuda.h>
#include <cuda_runtime.h>
#include <glm\glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <vector_types.h>


void ContactGen::addContact(bool isGPU){

	//von hier verden einzelne parts der koll.erkenn. angesteuert
	//auf cpu oder gpu
	if (isGPU == true){
		startGPU();
	}
	else{
		startCPU();
	}
}

void ContactGen::startCPU(){


}

void ContactGen::startGPU(){

	//cudaGetDevice(&temp);
	
	//arrays auf host anlegen
	glm::vec3 *h_pos;
	glm::quat *h_ori;
	//lin impuls
	//ang impuls

	//texturen auf device anlegen
	d_posTex;
	d_oriTex;

	

	//iterator über body-vector und werte in arrays schreiben


	//werte aus arrays in texturen übertragen
	cudaMemcpy(, h_pos, N*sizeof(float), cudaMemcpyHostToDevice);
	cudaMemcpy(, h_ori, N*sizeof(float), cudaMemcpyHostToDevice);
	cudaMemcpy(, h_array, N*sizeof(float), cudaMemcpyHostToDevice);
	cudaMemcpy(, h_array, N*sizeof(float), cudaMemcpyHostToDevice);
	
	//tests
	uint3 test;
	texture<float, 2, cudaReadModeElementType> oldPosTex;
	cudaTextureObject_t tex = 0;
	cudaCreateTextureObject();
}
*/
