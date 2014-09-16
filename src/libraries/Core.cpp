#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "Core.h"

Core::Core(){

	physicsWorld = new World;
	velocityIterations = NULL;		//festen wert setzten
	velocityEpsilon = NULL;			//festen wert setzten
	positionIterations = NULL;		//festen wert setzten
	positionEpsilon = NULL;			//festen wert setzten
	calcIterations = true;
	maxContacts = NULL;				//festen wert setzten
}

Core::~Core(){

	delete physicsWorld;
	delete velocityIterations;
	delete velocityEpsilon;
	delete positionIterations;
	delete positionEpsilon;
	delete calcIterations;
	delete maxContacts;
}

//mit constructor und createEngine etwas doppelt-gemoppelt
void Core::createEngine(double viIN, double veIN , double piIN, double peIN, bool ciIN, unsigned int mcIN){

	setVelocityIterations(viIN);
	setVelocityEpsilon(veIN);
	setPositionIterations(piIN);
	setPositionEpsilon(peIN);
	setCalcIterations(ciIN);
	setMaxContacts(mcIN);
}

//getter + setter
//siehe .h
