
// <<<<<<<<<< includes >>>>>>>>>> //
#include "UniformGrid2.h"
#include "World2.h"
#include "Particle2.h"

UniformGrid::UniformGrid(){
	voxelSize = 0;
	indexGrid = 0;
	countGrid = 0;
	gridLength = 0;
	gridMinPos = 0;
	partPerVoxel = 0;

	gridSize = 0;
	xSteps = 0;
	ySteps = 0;
}

UniformGrid::~UniformGrid(){

	//...
}

void UniformGrid::createGrid(){

	float worldS = World::getInstance()->getWorldSize();
	float partR = World::getInstance()->getPartRadius();

	voxelSize = 2.00f * partR;
	gridLength = (int)ceil(2.0f * worldS/ voxelSize);
	/* 2 extra grid voxels in each dimension in case particles go slightly outside the world borders */
	gridLength += 2;

	xSteps = gridLength*gridLength;
	ySteps = gridLength;

	std::cout << "Grid dimensions: " << gridLength << "*" << gridLength << "*" << gridLength << std::endl;

	gridMinPos = (-worldS) - voxelSize;
	//std::cout << "Grid minimum position: " << gridMinPos << std::endl;
	//std::cout << "Grid maximum position: " << gridMinPos + gridLength*voxelSize << std::endl;

	//std::cout << "Grid voxel size: " << voxelSize << std::endl;

	gridSize = gridLength * gridLength * gridLength;

	indexGrid = new int[gridSize*partPerVoxel];
	countGrid = new int[gridSize];

	for (int i=0; i<gridSize*partPerVoxel; i++) {
		indexGrid[i] = -1;
	}

	for (int i=0; i<gridSize; i++) {
		countGrid[i] = 0;
	}
}

void UniformGrid::updateGrid(){

	//partikel/body vektoren zu arrays ändern?!

	int aPartN = World::getInstance()->getAllPartNum();
	Particle* allPart = World::getInstance()->getAllParticles();	//schauen ob pointer so richtig

	for (int i=0; i<gridSize*partPerVoxel; i++) {
		indexGrid[i] = -1;
	}
	for (int i=0; i<gridSize; i++) {
		countGrid[i] = 0;
	}

	for (int i=0; i<aPartN; i++) {

		allPart[i]->updateGridIndex();
		int* gridIndex = allPart[i]->getGridIndex();

		if (isValidIndex(gridIndex)) {
			int flatCountGridIndex = gridIndex[0]*xSteps + gridIndex[1]*ySteps + gridIndex[2];
			int flatIndexGridIndex = flatCountGridIndex * partPerVoxel;

			indexGrid[flatIndexGridIndex + countGrid[flatCountGridIndex]] = i;

			countGrid[flatCountGridIndex]++;
		}
	}
}

