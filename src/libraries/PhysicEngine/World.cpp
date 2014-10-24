#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "World.h"
#include "Core.h"
#include "ForceRegistry.h"

World::World(){

	unsigned int temp = Core::getInstance()->getMaxContacts();
	contacts = new Contact[temp];
	forceReg = new ForceRegistry();
	contactGen = new ContactGen();
	resolver = new ContactResolver();
}

World::~World(){

	delete[] contacts;
	delete forceReg;
	delete contactGen;
	delete resolver;
	//delete bodies;
}

unsigned int World::genContacts(){

    unsigned limit = Core::getInstance()->getMaxContacts();

    Contact nextContact = contacts[0];

    for(int i; i<sizeof(contacts); i++){

    	/*
    	 unsigned used = reg->gen->addContact(nextContact, limit);
    	 limit -= used;
    	 nextContact += used;

    	 // We've run out of contacts to fill. This means we're missing
    	 // contacts.
    	 if (limit <= 0) break;
    	 */
    }

    //return the number of contacts used.
    return Core::getInstance()->getMaxContacts() - limit;
}

void World::runPhysics(float duration){

	//abändern für partikel algorithmus

	// First apply the force generators
	//registry.updateForces(duration);

	//integrate objects
	for(std::vector<RigidBody*>::iterator it = bodies.begin(); it != bodies.end(); ++it){

		(*it)->integrate(duration);
	}
	//generate contacts
	unsigned int numContacts = genContacts();

	// And process them
	/*
	if (Core::getInstance()->isCalcIterations() == true){

		resolver->setIterations(numContacts * 4);
	}
	*/
	resolver->resolveContacts(&contacts, numContacts, duration);
}

void World::startFrame(){

	for(std::vector<RigidBody*>::iterator it = bodies.begin(); it != bodies.end(); it++){

		//remove all forces from accumulator
		(*it)->clearAccu();
		(*it)->calcInternData();
	}
}

void World::addBody(RigidBody *body){

	bodies.push_back(body);
}
