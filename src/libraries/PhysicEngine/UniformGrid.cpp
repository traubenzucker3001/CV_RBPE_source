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

void UniformGrid::updateGrid(){		//!sollte korrekt sein! :debugging - cpu vers

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
		//cout << "" << gridIndex.x << 
		if (isValidIndex(gridIndex) == true) {
			//int flatCountGridIndex = (int)(gridIndex.x*xSteps + gridIndex.y*ySteps + gridIndex.z);
			int flatCountGridIndex = (int)gridIndex.x*xSteps + (int)gridIndex.y*ySteps + (int)gridIndex.z;
			int flatIndexGridIndex = flatCountGridIndex * partPerVoxel;

			gridCells[flatIndexGridIndex + gridCounters[flatCountGridIndex]] = i;
			
			//for (int i = 0; i < gridSize*4; i++){	//zum debuggen
			//	cout << "gridCell " << i << ": " << gridCells[i] << endl;	//zum debuggen
			//	if (gridCells[i] != -1){
			//		cout << "!!HIER!!" << endl;
			//	}
			//}	//zum debuggen
			

			gridCounters[flatCountGridIndex]++;
			
			//for (int i = 0; i < gridSize; i++){	//zum debuggen
			//	cout << "gridCounter " << i << ": " << gridCounters[i] << endl;	//zum debuggen
			//	if (gridCounters[i] != 0){
			//		cout << "!!HIER!!" << endl;
			//	}
			//}	//zum debuggen
			
		}
	}
	
	int temp = world->getAllPartNum();
	for (int i = 0; i < temp; i++){
		glm::vec3 pos = world->allParticles[i]->getPosition();
		glm::vec3 grid = world->allParticles[i]->getGridIndex();
		//cout << "partpos " << i << ": " << pos.x << ", " << pos.y << ", " << pos.z << endl;	//zum debuggen
		//cout << "partgrid " << i << ": " << grid.x << ", " << grid.y << ", " << grid.z << endl;	//zum debuggen
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

int* UniformGrid::getNeighborPartIndices(glm::vec3 gridIndex){		//<--da drin müsste fehler liegen!!

	//cout << "grid: get neighbors!" << endl; //zum test

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

	/*
	for (int i = 0; i < gridSize*4; i++){	//zum debuggen
		cout << "gridCell " << i << ": " << gridCells[i] << endl;	//zum debuggen
		if (gridCells[i] != -1){
			cout << "!!HIER!!" << endl;
		}
	}	//zum debuggen
	for (int i = 0; i < gridSize; i++){	//zum debuggen
		cout << "gridCounter " << i << ": " << gridCounters[i] << endl;	//zum debuggen
		if (gridCounters[i] != 0){
			cout << "!!HIER!!" << endl;
		}
	}	//zum debuggen
	*/

	glm::vec3 checkIndex = glm::vec3(gridIndex.x - 1, gridIndex.y - 1, gridIndex.z - 1);	//vllt mit ivec (int vector) versuchen
	//glm::vec3 checkIndex = glm::vec3(gridIndex.x, gridIndex.y, gridIndex.z );
	//cout << "checki: " << checkIndex.x << ", " << checkIndex.y << ", " << checkIndex.z << endl;	//zum debuggen
	int neighborCount = 0;
	int* indices = new int[27 * partPerVoxel];

	//for (int i = 0; i < 27 * 4; i++){	//zum debuggen
	//	cout << "nachbar " << i << ": " << indices[i] << endl;	//zum debuggen
	//}	//zum debuggen

	for (int ix = 0; ix < 3; ix++) {
		checkIndex.y = gridIndex.y - 1; //reset y index before y-loop

		//cout << "checki: " << checkIndex.x << ", " << checkIndex.y << ", " << checkIndex.z << endl;	//zum debuggen

		for (int iy = 0; iy < 3; iy++) {
			checkIndex.z = gridIndex.z - 1; //reset z index before z-loop

			//cout << "checki: " << checkIndex.x << ", " << checkIndex.y << ", " << checkIndex.z << endl;	//zum debuggen

			for (int iz = 0; iz < 3; iz++) {
				int flatCountGridIndex = (int)checkIndex.x*xSteps + (int)checkIndex.y*ySteps + (int)checkIndex.z;
				//int flatCountGridIndex = (int)(checkIndex.x*xSteps + checkIndex.y*ySteps + checkIndex.z);
				//cout << "(int): " << (int)checkIndex.x*xSteps << endl;	//zum debuggen
				//cout << "(int): " << (int)checkIndex.y*ySteps << endl;	//zum debuggen
				//cout << "(int): " << (int)checkIndex.z << endl;	//zum debuggen
				//cout << "flatCountGridIndex: " << flatCountGridIndex << endl;	//zum debuggen
				int flatIndexGridIndex = flatCountGridIndex * partPerVoxel;
				//cout << "flatIndexGridIndex: " << flatIndexGridIndex << endl;	//zum debuggen

				//for (int i = 0; i < 27 * 4; i++){	//zum debuggen
				//	cout << "nachbar " << i << ": " << indices[i] << endl;	//zum debuggen
				//}	//zum debuggen

				//cout << "neighborCount: " << neighborCount << endl;		//zum debuggen
				indices[neighborCount] = gridCells[flatIndexGridIndex];
				indices[neighborCount + 1] = gridCells[flatIndexGridIndex + 1];
				indices[neighborCount + 2] = gridCells[flatIndexGridIndex + 2];
				indices[neighborCount + 3] = gridCells[flatIndexGridIndex + 3];

				//^^^^davor muss fehler liegen^^^^ , alle einträge sind -1
				//for (int i = 0; i < 27 * 4; i++){	//zum debuggen
				//	cout << "nachbar " << i << ": " << indices[i] << endl;	//zum debuggen
				//	if (indices[i]==2){
				//		cout << "!!HIER!!" << endl;
				//	}
				//	if (indices[i] == 27){
				//		cout << "!!HIER!!" << endl;
				//	}
				//}	//zum debuggen

				neighborCount = neighborCount + 4;
				checkIndex.z++;
			}
			checkIndex.y++;
		}
		checkIndex.x++;
	}
	/*
	for (int i = 0; i < 27 * 4; i++){	//zum debuggen
		cout << "nachbar " << i << ": " << indices[i] << endl;	//zum debuggen
		if (indices[i] != -1){
			cout << "!!HIER!!" << endl;	//zum debuggen
		}
	}	//zum debuggen
	*/
	return indices;
}
