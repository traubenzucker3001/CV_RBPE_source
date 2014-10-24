#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "Box.h"
#include "DemoApp/Demo.h"

using namespace std;

Box::~Box(){

}

void Box::calcParticles(){

	cout << "box" << endl;
	//bodyParticle = new Particle*[bPartNum]; //muss aber vektor

	//int i=0;
	int temp = Demo::getInstance()->getPartRadius();
	float space = temp*2.0f;
	for (int j=-1; j<=1; j++) {
		for (int k=-1; k<=1; k++) {
			for (int l=-1; l<=1; l++) {
				float x = origin.x+space*j;
				float y = origin.y+space*k;
				float z = origin.z+space*l;
				glm::vec3 particlePos = glm::vec3(x,y,z);

				//bodyParticle[i] = new Particle(particlePos, mass/bPartNum);		//weg und nur drunter stehendes nehmen
				Particle *particle = new Particle(particlePos, mass/bPartNum);
				bodyParticle.push_back(particle);
				//i++;
			}
		}
	}
}

void Box::applyRotatToPart(glm::mat3 rotMatrix) {
	int i=0;
	int temp = Demo::getInstance()->getPartRadius();
	float space = temp*2.0f;
	for (int j=-1; j<=1; j++) {
		for (int k=-1; k<=1; k++) {
			for (int l=-1; l<=1; l++) {
				float x = space*j;
				float y = space*k;
				float z = space*l;
				float originalRelativePos[3] = {x, y, z};

				particles[i]->applyRotation(rotMatrix, originalRelativePos, origin); //vector zugriff statt array

				i++;
			}
		}
	}
}
