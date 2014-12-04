
// <<<<<<<<<< includes >>>>>>>>>> //
#include "World.h"
#include "UniformGrid.h"
#include "RigidBody.h"
#include "Particle.h"

World::World(float wsIN, float prIN, float scIN, float dcIN, int bNum){

	cout << "world: world constr called!" << endl; //zum test

	worldSize = wsIN;
	partRadius = prIN;
	springCoeff = scIN;
	dampCoeff = dcIN;
	gravity = 9.81f;			//fester wert
	allBodyNum = bNum;
	allBodies = new RigidBody*[allBodyNum];
	allPartNum = bNum * 27;
	allParticles = new Particle*[allPartNum];
	grid = new UniformGrid();
	cudaStruct = new Cuda(allBodyNum, allPartNum);
}

World::~World(){

	delete grid;
	delete cudaStruct;
	delete allParticles;
	delete allBodies;
}

void World::stepPhysics(float duration, bool isGPU){

	cout << "world: step physics!" << endl; //zum test

	//unterteilen in cpu und gpu

	//ausführung auf gpu
	if(isGPU == true){
		Cuda::getInstance()->stepCUDA();
	}
	//ausführung auf cpu
	else{

		//update part. values
		//for(std::vector<RigidBody*>::iterator it = allBodies.begin(); it != allBodies.end(); ++it){
		for (int i = 0; i < allBodyNum; i++){
			//(*it)->updatePartValues();			//in array geändert
			allBodies[i]->updatePartValues();
		}	

		//update grid
		UniformGrid::getInstance()->updateGrid();

		//update momenta
		//for(std::vector<RigidBody*>::iterator it = allBodies.begin(); it != allBodies.end(); ++it){
		for (int i = 0; i < allBodyNum; i++){
			//(*it)->updateMomenta(duration);		//in array geändert
			allBodies[i]->updateMomenta(duration);
		}

		//iterate
		//for(std::vector<RigidBody*>::iterator it = allBodies.begin(); it != allBodies.end(); ++it){
		for (int i = 0; i < allBodyNum; i++){
			//(*it)->iterate(duration);				//in array geändert
			allBodies[i]->iterate(duration);
		}

		//VOs updaten
		//TODO
	}
}
