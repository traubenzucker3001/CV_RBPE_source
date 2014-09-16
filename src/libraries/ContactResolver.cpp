//
#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

//
#include "ContactResolver.h"
#include "Core.h"

ContactResolver::ContactResolver(){

	velocityIterations = Core::getInstance()->getVelocityIterations();
	velocityEpsilon = Core::getInstance()->getVelocityEpsilon();
	positionIterations = Core::getInstance()->getPositionIterations();
	positionEpsilon = Core::getInstance()->getPositionEpsilon();

	if(Core::getInstance()->isCalcIterations() == true){
		calcVelocIter = 0;
		calcPosIter = 0;
	}
	else{
		calcVelocIter = NULL;
		calcPosIter = NULL;
	}
}

ContactResolver::~ContactResolver(){

	delete velocityIterations;
	delete velocityEpsilon
	delete positionIterations;
	delete positionEpsilon;
}

void ContactResolver::prepareContacts(Contact *contArray, unsigned int numCont, double duration){

	Contact* lastContact = contArray + numCont;
	for(Contact* contact=contArray; contact < lastContact; contact++){

	        contact->calcInternData(duration);
	    }
}

void ContactResolver::resolveContacts(Contact *contArray, unsigned int numCont, double duration){

	if (numCont == 0){
		return;
	}
	prepareContacts(contArray, numCont, duration);

	adjustPos(contArray, numCont, duration);

	adjustVeloc(contArray, numCont, duration);
}

void ContactResolver::adjustVeloc(Contact *contArray, unsigned int numContacts, double duration){

}

void ContactResolver::adjustPos(Contact *contArray, unsigned int numContacts, double duration){

}
