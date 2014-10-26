
// <<<<<<<<<< includes >>>>>>>>>> //
#include "World2.h"
#include "UniformGrid2.h"
#include "RigidBody2.h"

World::World(float wsIN, float prIN, float scIN, float dcIN){

	worldSize = wsIN;
	partRadius = prIN;
	springCoeff = scIN;
	dampCoeff = dcIN;
	gravity = 9.81;			//fester wert
	allbodyNum = 0;
	allParticles = 0;
	allPartNum = 0;
	grid = new UniformGrid();

}

World::~World(){

	//...
}

void World::stepPhysics(float duration){

	//update part. values
	for(std::vector<RigidBody*>::iterator it = allBodies.begin(); it != allBodies.end(); ++it){
		(*it)->updatePartValues();
	}

	//update grid
	UniformGrid::getInstance()->updateGrid();

	//update momenta
	for(std::vector<RigidBody*>::iterator it = allBodies.begin(); it != allBodies.end(); ++it){
		(*it)->updateMomenta(duration);
	}

	//iterate
	for(std::vector<RigidBody*>::iterator it = allBodies.begin(); it != allBodies.end(); ++it){
		(*it)->iterate(duration);
	}
}
