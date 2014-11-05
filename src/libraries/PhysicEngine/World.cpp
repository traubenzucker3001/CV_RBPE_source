
// <<<<<<<<<< includes >>>>>>>>>> //
#include "World.h"
#include "UniformGrid.h"
#include "RigidBody.h"

World::World(float wsIN, float prIN, float scIN, float dcIN){

	worldSize = wsIN;
	partRadius = prIN;
	springCoeff = scIN;
	dampCoeff = dcIN;
	gravity = 9.81f;			//fester wert
	allbodyNum = 0;
	allParticles = 0;
	allPartNum = 0;
	grid = new UniformGrid();

}

World::~World(){

	//...
}

void World::stepPhysics(float duration, bool isGPU){

	//unterteilen in cpu und gpu

	//ausführung auf gpu
	if(isGPU == true){
		Cuda::getInstance()->stepCUDA();
	}
	//ausführung auf cpu
	else{

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
}
