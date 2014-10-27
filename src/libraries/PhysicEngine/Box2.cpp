//!noch partikel array zugriffs fehler!

// <<<<<<<<<< includes >>>>>>>>>> //
#include "Box2.h"
#include "Particle2.h"
#include "World2.h"

Box::~Box(){

	//...
}

void Box::calcParticles(){

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
				float particlePos[3] = {x,y,z};
				bodyParticles[i] = new Particle(particlePos, mass/numOfPart);
				i++;
			}
		}
	}
}

void Box::applyRotToPart(glm::mat3 rotMatrix){

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

	for (int i=0; i<numOfPart; i++) {
		bodyParticles[i]->populateArray();
	}
}
