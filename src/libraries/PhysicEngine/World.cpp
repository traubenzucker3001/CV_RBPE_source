
// <<<<<<<<<< includes >>>>>>>>>> //
#include "World.h"
#include "UniformGrid.h"
#include "RigidBody.h"
#include "Particle.h"

//link fix try 4
Cuda* cuda;

World::World(float wsIN, float prIN, float scIN, float dcIN, int bNum){

	worldSize = wsIN;
	partRadius = prIN;
	springCoeff = scIN;
	dampCoeff = dcIN;
	gravity = 9.81f;
	allBodyNum = bNum;
	allBodies = new RigidBody*[allBodyNum];
	allPartNum = bNum * 27;
	allParticles = new Particle*[allPartNum];		
	grid = new UniformGrid();
	cuda = new Cuda(allBodyNum, allPartNum);
}

World::~World(){

	delete grid;
	delete allParticles;
	delete allBodies;
}

void World::stepPhysics(float duration, bool isGPU){

	//unterteilen in cpu und gpu
	//ausführung auf gpu
	if(isGPU == true){
		cuda->stepCUDA();
	}
	//ausführung auf cpu
	else{

		//update part. values
		for (int i = 0; i < allBodyNum; i++){

			allBodies[i]->updatePartValues();
		}	

		//update grid
		UniformGrid::getInstance()->updateGrid();

		//update momenta
		for (int i = 0; i < allBodyNum; i++){

			allBodies[i]->updateMomenta(duration);
		}

		//iterate
		for (int i = 0; i < allBodyNum; i++){

			allBodies[i]->iterate(duration);
		}
	}
}
