//!noch partikel array zugriffs fehler!

// <<<<<<<<<< includes >>>>>>>>>> //
#include <iostream>
#include "Box.h"
#include "Particle.h"
#include "World.h"

using namespace std;

Box::~Box(){

	//no pointers to clear!

}

void Box::calcParticles(){

	cout << "box: calcParticles called!" << endl; //zum test

	bodyParticles = new Particle*[numOfPart];
	int i=0;
	float partR = World::getInstance()->getPartRadius();
	float space = partR * 2.0f;
	for (int j=-1; j<=1; j++) {
		for (int k=-1; k<=1; k++) {
			for (int l=-1; l<=1; l++) {
				float x = origin.x + space * j;
				float y = origin.y + space * k;
				float z = origin.z + space * l;
				glm::vec3 particlePos = glm::vec3(x,y,z);
				bodyParticles[i] = new Particle(particlePos, mass/numOfPart);
				i++;
			}
		}
	}
}

void Box::applyRotToPart(glm::mat3 rotMatrix){

	cout << "box: applyRoToPart called!" << endl; //zum test

	int i=0;
	float partR = World::getInstance()->getPartRadius();
	float space = partR * 2.0f;
	for (int j=-1; j<=1; j++) {
		for (int k=-1; k<=1; k++) {
			for (int l=-1; l<=1; l++) {
				float x = space*j;
				float y = space*k;
				float z = space*l;
				//float originalRelativePos[3] = {x, y, z};
				glm::vec3 relatPos = glm::vec3(x,y,z);

				bodyParticles[i]->applyRot(rotMatrix, relatPos, origin);
				i++;
			}
		}
	}
}

void Box::populatePartArray(){

	cout << "box: populPartAr called!" << endl; //zum test

	for (int i=0; i<numOfPart; i++) {
		bodyParticles[i]->populateArray();
	}
}
