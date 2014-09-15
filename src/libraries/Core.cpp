#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "Core.h"

//mal sehen ob überhaupt nötig
Core::Core(){

	physicsWorld = new World;
	//velocityIterations = ;		//festen wert setzten
	//velocityEpsilon = ;			//festen wert setzten
	//positionIterations = ;		//festen wert setzten
	//positionEpsilon = ;			//festen wert setzten
	calcIterations = true;
	//maxContacts = ;				//festen wert setzten
}

//mal sehen ob überhaupt nötig
Core::Core(double viIN, double veIN , double piIN, double peIN, bool ciIN, unsigned int mcIN){

	physicsWorld = new World;
	velocityIterations = viIN;
	velocityEpsilon = veIN;
	positionIterations = piIN;
	positionEpsilon = peIN;
	calcIterations = true;
	maxContacts = mcIN;
}

//mal sehen ob überhaupt nötig
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


}
