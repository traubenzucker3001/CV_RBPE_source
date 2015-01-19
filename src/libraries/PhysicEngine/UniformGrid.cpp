
// <<<<<<<<<< includes >>>>>>>>>> //
#include "UniformGrid.h"
#include "World.h"
#include "Particle.h"
#include "DemoApp\Demo.h"

//link fix try 4
extern World* world;
extern Demo* demo;

UniformGrid::UniformGrid(){

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

	float worldS = world->getWorldSize();
	float partR = world->getPartRadius();

	voxelSize = 2.00f * partR;
	gridLength = (int)ceil(2.0f * worldS / voxelSize);

	/* 2 extra grid voxels in each dimension in case particles go slightly outside the world borders */
	gridLength = gridLength + 2;

	xSteps = gridLength*gridLength;
	ySteps = gridLength;

	//std::cout << "Grid dimensions: " << gridLength << "*" << gridLength << "*" << gridLength << std::endl;

	gridMinPos = -worldS - voxelSize;

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

	int aPartN = world->getAllPartNum();

	for (int i=0; i<gridSize*partPerVoxel; i++) {
		gridCells[i] = -1;
	}
	for (int i=0; i<gridSize; i++) {
		gridCounters[i] = 0;
	}

	for (int i=0; i<aPartN; i++) {

		world->allParticles[i]->updateGridIndex();
		
		glm::ivec3 gridIndex = world->allParticles[i]->getGridIndex();
		
		if (isValidGridIndex(gridIndex) == true) {
			
			int flatCountGridIndex = gridIndex.x*xSteps + gridIndex.y*ySteps + gridIndex.z;
			int flatIndexGridIndex = flatCountGridIndex * partPerVoxel;

			gridCells[flatIndexGridIndex + gridCounters[flatCountGridIndex]] = i;
			
			gridCounters[flatCountGridIndex]++;
			
		}
	}
}

bool UniformGrid::isValidGridIndex(glm::ivec3 gridIndex){

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

int* UniformGrid::getNeighborPartIndices(glm::ivec3 gridIndex){

	//If you're at the border, go 1 cell inwards to prevent going out of bounds
	if (gridIndex.x == 0) {
		gridIndex.x++;
	}
	else if (gridIndex.x == (gridLength - 1)) {
		gridIndex.x--;
	}
	if (gridIndex.y == 0) {
		gridIndex.y++;
	}
	else if (gridIndex.y == (gridLength - 1)) {
		gridIndex.y--;
	}
	if (gridIndex.z == 0) {
		gridIndex.z++;
	}
	else if (gridIndex.z == (gridLength - 1)) {
		gridIndex.z--;
	}

	glm::ivec3 checkIndex = glm::ivec3(gridIndex.x - 1, gridIndex.y - 1, gridIndex.z - 1);
	
	int neighborCount = 0;
	int* indices = new int[8 * partPerVoxel];	//27

	for (int ix = 0; ix < 3; ix++) {
		checkIndex.y = gridIndex.y - 1; //reset y index before y-loop

		for (int iy = 0; iy < 3; iy++) {
			checkIndex.z = gridIndex.z - 1; //reset z index before z-loop

			for (int iz = 0; iz < 3; iz++) {
		
				int flatCountGridIndex = checkIndex.x*xSteps + checkIndex.y*ySteps + checkIndex.z;
				
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
