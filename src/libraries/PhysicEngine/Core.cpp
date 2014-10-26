/*
#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include <cuda.h>
#include <cuda_runtime.h>

#include "Core.h"

Core::Core(){

	physicsWorld = new World();
	velocityIterations = NULL;
	velocityEpsilon = NULL;
	positionIterations = NULL;
	positionEpsilon = NULL;
	calcIterations = NULL;
	maxContacts = NULL;
	deviceID = 0;

}

Core::Core(float viIN, float veIN , float piIN, float peIN, bool ciIN, unsigned int mcIN){

	physicsWorld = new World();
	velocityIterations = viIN;
	velocityEpsilon = veIN;
	positionIterations = piIN;
	positionEpsilon = peIN;
	calcIterations = ciIN;
	maxContacts = mcIN;
	deviceID = 0;
}

Core::~Core(){

	delete physicsWorld;
	//delete velocityIterations;
	//delete velocityEpsilon;
	//delete positionIterations;
	//delete positionEpsilon;
	//delete calcIterations;
	//delete maxContacts;
}

/*
//mit constructor und createEngine etwas doppelt-gemoppelt
//geändert in change engine
//ändern kann man aber auch einfach per setter, also immernoch nicht optimal
void Core::changeEngine(double viIN, double veIN , double piIN, double peIN, bool ciIN, unsigned int mcIN){

	setVelocityIterations(viIN);
	setVelocityEpsilon(veIN);
	setPositionIterations(piIN);
	setPositionEpsilon(peIN);
	setCalcIterations(ciIN);
	setMaxContacts(mcIN);
}
*/

/*void Core::initCUDA(){
	cudaGetDevice(&deviceID);

	if (deviceID < 0){
		printf("kein CUDA kompatibles Gerät gefunden found\n");
		//exit(EXIT_SUCCESS);
	}
}

// <<<<<<<<<< getter + setter >>>>>>>>>> //
//siehe Core.h
*/
