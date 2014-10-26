/*
#include <iostream>
#include <math.h>

#include "UniformGrid2.h"

using namespace std;

//hilfsvar.
int xSteps;
int ySteps;
int gridSize;

void createGrid() {

	float temp = Demo::getInstance()->getPartRadius();
	//voxelSize = 2.00f*particleRadius;
	voxelSize = 2.00f * temp;

	//gridLength = (int)ceil(2.0f * worldSize / voxelSize);
	gridLength = (int)ceil(2.0f * Demo::getInstance()->getWorldSize() / voxelSize);

	/* 2 extra grid voxels in each dimension in case particles go slightly outside the world borders */
/*	gridLength += 2;

	xSteps = gridLength*gridLength;
	ySteps = gridLength;

	std::cout << "gitter dimensionen: " << gridLength << "*" << gridLength << "*" << gridLength << std::endl;

	float temp2 = Demo::getInstance()->getWorldSize();
	gridMinimumPosition = -temp2 - voxelSize;

	std::cout << "gitter min position: " << gridMinimumPosition << std::endl;
	std::cout << "gitter max position: " << gridMinimumPosition + gridLength*voxelSize << std::endl;

	std::cout << "gitter voxel größe: " << voxelSize << std::endl;

	gridSize = gridLength * gridLength * gridLength;

	indexGrid = new int[gridSize*particlesPerVoxel];
	countGrid = new int[gridSize];

	for (int i=0; i<gridSize*particlesPerVoxel; i++) {
		indexGrid[i] = -1;
	}

	for (int i=0; i<gridSize; i++) {
		countGrid[i] = 0;
	}
}

void updateGrid() {
	for (int i=0; i<gridSize*particlesPerVoxel; i++) {
		indexGrid[i] = -1;
	}
	for (int i=0; i<gridSize; i++) {
		countGrid[i] = 0;
	}

	//for (int i=0; i<partNum; i++) {
	std::vector<Particle*> temp = World::getInstance()->getAllParticles();
	for(std::vector<Particle*>::iterator it = temp.begin(); it != temp.end(); ++it){

		int i = 0;

		(*it)->updateGridIndex();
		int gridIndex = (*it)->getGridIndex();

		if (isValidIndex(gridIndex)) {
			int flatCountGridIndex = gridIndex[0]*xSteps + gridIndex[1]*ySteps + gridIndex[2];
			int flatIndexGridIndex = flatCountGridIndex * particlesPerVoxel;

			//da ja kein array, oben hilfsvar i eingefügt
			indexGrid[flatIndexGridIndex + countGrid[flatCountGridIndex]] = i;

			countGrid[flatCountGridIndex]++;
			}
	}
}
*/