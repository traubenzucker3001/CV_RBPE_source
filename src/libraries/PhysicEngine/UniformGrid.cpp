//!noch particle array zugriffs fehler!

// <<<<<<<<<< includes >>>>>>>>>> //
#include "UniformGrid.h"
#include "World.h"
#include "Particle.h"
#include "DemoApp\Demo.h"

//link fix try 4
extern World* world;
extern Demo* demo;

UniformGrid::UniformGrid(){
	
	//cout << "grid: grid constr. called!" << endl; //zum test

	voxelSize = 0;
	gridCells = 0;
	gridCounters = 0;
	gridLength = 0;
	gridMinPos = 0;
	partPerVoxel = 4;

	gridSize = 0;
	xSteps = 0;
	ySteps = 0;
}

UniformGrid::~UniformGrid(){

	delete gridCells;
	delete gridCounters;
}

void UniformGrid::createGrid(){

	//cout << "grid: create grid!" << endl; //zum test

	float worldS = world->getWorldSize();
	float partR = world->getPartRadius();

	voxelSize = 2.00f * partR;
	gridLength = (int)ceil(2.0f * worldS / voxelSize);

	/* 2 extra grid voxels in each dimension in case particles go slightly outside the world borders */
	gridLength = gridLength + 2;

	xSteps = gridLength*gridLength;
	ySteps = gridLength;

	std::cout << "Grid dimensions: " << gridLength << "*" << gridLength << "*" << gridLength << std::endl;

	gridMinPos = (-worldS) - voxelSize;
	//std::cout << "Grid minimum position: " << gridMinPos << std::endl;
	//std::cout << "Grid maximum position: " << gridMinPos + gridLength*voxelSize << std::endl;

	//std::cout << "Grid voxel size: " << voxelSize << std::endl;

	gridSize = gridLength * gridLength * gridLength;

	bool gpu = demo->isIsGpu();
	if (gpu == false){

		gridCells = new int[gridSize*partPerVoxel];
		gridCounters = new int[gridSize];
	
		for (int i=0; i<gridSize*partPerVoxel; i++) {
			gridCells[i] = -1;
		}

		for (int i=0; i<gridSize; i++) {
			gridCounters[i] = 0;
		}
	}
}

void UniformGrid::updateGrid(){

	//cout << "grid: update grid!" << endl; //zum test

	int aPartN = world->getAllPartNum();
	//Particle** allPart = world->getAllParticles();

	for (int i=0; i<gridSize*partPerVoxel; i++) {
		gridCells[i] = -1;
	}
	for (int i=0; i<gridSize; i++) {
		gridCounters[i] = 0;
	}

	for (int i=0; i<aPartN; i++) {

		//allPart[i]->updateGridIndex();
		world->allParticles[i]->updateGridIndex();
		//glm::vec3 gridIndex = allPart[i]->getGridIndex();
		glm::vec3 gridIndex = world->allParticles[i]->getGridIndex();

		if (isValidIndex(gridIndex) == true) {
			int flatCountGridIndex = (int)(gridIndex.x*xSteps + gridIndex.y*ySteps + gridIndex.z);
			int flatIndexGridIndex = flatCountGridIndex * partPerVoxel;

			gridCells[flatIndexGridIndex + gridCounters[flatCountGridIndex]] = i;

			gridCounters[flatCountGridIndex]++;
		}
	}
}

bool UniformGrid::isValidIndex(glm::vec3 gridIndex){

	//cout << "grid: valid grid index?!" << endl; //zum test

	bool valid = true;

	if (gridIndex.x >= gridLength || gridIndex.x < 0) {
		valid= false;
	}
	if (gridIndex.y >= gridLength || gridIndex.y < 0) {
		valid= false;
	}
	if (gridIndex.z >= gridLength || gridIndex.z < 0) {
		valid= false;
	}
	return valid;
}

int* UniformGrid::getNeighborPartIndices(glm::vec3 gridIndex){

	//cout << "grid: get neighbors!" << endl; //zum test

	//If you're at the border, go 1 cell inwards to prevent going out of bounds
	if (gridIndex.x == 0) {
		gridIndex.x++;
	} else if (gridIndex.x == (gridLength-1)) {
		gridIndex.x--;
	}
	if (gridIndex.y == 0) {
		gridIndex.y++;
	} else if (gridIndex.y == (gridLength-1)) {
		gridIndex.y--;
	}
	if (gridIndex.z == 0) {
		gridIndex.z++;
	} else if (gridIndex.z == (gridLength-1)) {
		gridIndex.z--;
	}

	glm::vec3 checkIndex = glm::vec3(gridIndex.x-1,gridIndex.y-1,gridIndex.z-1);
	//glm::vec3 checkIndex = glm::vec3(gridIndex.x, gridIndex.y, gridIndex.z );

	int neighborCount = 0;
	int* indices = new int[27*partPerVoxel];

	for (int ix=0; ix<3; ix++) {
		checkIndex.y = gridIndex.y - 1; //reset y index before y-loop

		for (int iy=0; iy<3; iy++) {
			checkIndex.z = gridIndex.z-1; //reset z index before z-loop

			for (int iz=0; iz<3; iz++) {
				int flatCountGridIndex = (int)checkIndex.x*xSteps + (int)checkIndex.y*ySteps + (int)checkIndex.z;
				int flatIndexGridIndex = flatCountGridIndex * partPerVoxel;
	
				indices[neighborCount] = gridCells[flatIndexGridIndex];
				indices[neighborCount + 1] = gridCells[flatIndexGridIndex + 1];
				indices[neighborCount + 2] = gridCells[flatIndexGridIndex + 2];
				indices[neighborCount + 3] = gridCells[flatIndexGridIndex + 3];
		
				neighborCount = neighborCount + 4;
				checkIndex.z++;
			}
			checkIndex.y++;
		}
		checkIndex.x++;
	}
	return indices;
}
