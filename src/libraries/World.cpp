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
	forceReg = new ForceRegistry;
	contactGen = new ContactGen;
	resolver = new ContactResolver;
	//bodies = ;

}

World::~World(){

	delete[] contacts;
	delete forceReg;
	delete contactGen;
	delete resolver;
	delete bodies;
}

void World::genContacts(){


}

void World::runPhysics(double duration){



}

void World::startFrame(){

	RigidBodyReg *bodyReg = bodies;
	while (bodyReg){
		//remove all forces from accumulator
		bodyReg->body->clearAccu();
		bodyReg->body->calcInternData();

		//get next body
		bodyReg = bodyReg->next;
	}
}
