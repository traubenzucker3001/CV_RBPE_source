
// <<<<<<<<<< includes >>>>>>>>>> //
#include "World.h"
#include "UniformGrid.h"
#include "RigidBody.h"
#include "Particle.h"

//link fix try 4
Cuda* cuda;

World::World(float wsIN, float prIN, float scIN, float dcIN, int bNum){

	//cout << "world: world constr called!" << endl; //zum test

	worldSize = wsIN;
	partRadius = prIN;
	springCoeff = scIN;
	dampCoeff = dcIN;
	gravity = 9.81f;			//fester wert
	allBodyNum = bNum;
	allBodies = new RigidBody*[allBodyNum];
	allPartNum = bNum * 27;
	allParticles = new Particle*[allPartNum];		//mall alle position ausgeben lassen
	grid = new UniformGrid();
	//cudaStruct = new Cuda(allBodyNum, allPartNum);
	cuda = new Cuda(allBodyNum, allPartNum);
}

World::~World(){

	delete grid;
	//delete cudaStruct;
	delete allParticles;
	delete allBodies;
}

void World::stepPhysics(float duration, bool isGPU){

	//cout << "world: step physics!" << endl; //zum test

	//unterteilen in cpu und gpu
	//ausf�hrung auf gpu
	if(isGPU == true){
		cuda->stepCUDA();
	}
	//ausf�hrung auf cpu
	else{

		//update part. values
		for (int i = 0; i < allBodyNum; i++){

			allBodies[i]->updatePartValues();
		}	

		//update grid
		UniformGrid::getInstance()->updateGrid();

		//update momenta
		for (int i = 0; i < allBodyNum; i++){

			//cout << "i: " << i << endl;	//zum debuggen
			allBodies[i]->updateMomenta(duration);
		}

		//iterate
		for (int i = 0; i < allBodyNum; i++){

			allBodies[i]->iterate(duration);
		}
	}
}
