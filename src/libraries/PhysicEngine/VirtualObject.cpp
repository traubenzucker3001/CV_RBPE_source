
// <<<<<<<<<< includes >>>>>>>>>> //
#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>
#include <glm\glm.hpp>

#include <cuda.h>
#include <cuda_runtime.h>

#include "VirtualObject.h"
#include "DemoApp\Demo.h"
#include "World.h"
#include "Cuda.h"

//git branch test
//link fix try 4
extern World* world;

using namespace std;

//link fix try 4
extern Demo* demo;
extern Cuda* cuda;

VirtualObject::VirtualObject(glm::vec3 posIN, int bodyCount, float massIN, bool staticIN, bool shapeIN, float sizeIN, int idIN){

	//	cout << "vo: vo constr called!" << endl; //zum test

	id = idIN;
	position = posIN;
	//cout << "posVO: " << position.y << endl;	//zum debuggen
	physicBody = new RigidBody(massIN, staticIN, shapeIN, posIN,sizeIN);

	//schauen ob body/part world arrays richtig bzw. überhaupt befüllt werden!!!
	world->allBodies[bodyCount] = physicBody;

	graphicBody = new CVK::Node();
	//material setzten, funktioniert noch nicht ganz
	//CVK::Material mat_cvlogo((char*)RESOURCES_PATH "/cv_logo.bmp", black, grey, 100.0f);
	//todo: anderes material, logo wird irgwie nicht angezeigt. vllt auch auslagern dass nur es einmal vorliegt
	//CVK::Material mat = demo->cubeMaterial;

	graphicBody->setGeometry(demo->cubeGeometry);
	graphicBody->setMaterial((demo->cubeMaterial));

	//liegen alle aufeinander weil hier pos nicht berücksichtigt wird
	//graphicBody->setModelMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0.72, 0)));
	graphicBody->setModelMatrix(glm::translate(glm::mat4(1.0f), position));
	demo->sceneRoot->addChild(graphicBody);
}

VirtualObject::~VirtualObject(){

	delete physicBody;
	delete graphicBody;
}

//über cuda - opengl austausch
void VirtualObject::updateCPU(){

	//cout << "vo: update cpu!" << endl; //zum test

	//update modelmatrix with new rb values
	//RigidBody *tempB = world->allBodies[id];		
	//glm::vec3 tempP = tempB->getPosition();
	glm::vec3 tempP = world->allBodies[id]->getPosition();
	glm::quat tempQ = world->allBodies[id]->getRotationQuat();
	//glm::mat3 tempO = tempB->getRotationMat();
	//glm::quat tempQ = tempB->getRotationQuat();
	//modelmatrix neu berechnen
	glm::mat4 t = glm::translate(glm::mat4(1.0f), tempP);
	
	//!!rotations matrix umwandeln (mat3 zu mat4)!! oder quat zu mat4
	//glm::mat4 r = glm::mat4(tempO, 0.0f);
	glm::mat4 r = glm::toMat4(tempQ);			//mal schaun ob so funktioniert!!?

	modelMatrix = t * r;

	//set new modelmatrix from node
	graphicBody->setModelMatrix(modelMatrix);
}

void VirtualObject::updateGPU(){

	//cout << "vo: update gpu!" << endl; //zum test

	//werte von gpu beschaffen
	glm::vec3 tempP = cuda->h_uVOpos[id];
	//cout << cuda->h_uVOpos[id].y << endl;	//zum debuggen
	//glm::mat3 tempO;
	glm::quat tempQ = cuda->h_uVOrot[id];

	//cudaMemcpyFromSymbol	//nicht hier, alle benötigten daten in h_array und dann daraus werte holen

	//modelmatrix neu berechnen
	glm::mat4 t = glm::translate(glm::mat4(1.0f), tempP);

	glm::mat4 r = glm::toMat4(tempQ);			//mal schaun ob so funktioniert!!?

	modelMatrix = t * r;
	//cout << "test" << endl; //zum debuggen
	//set new modelmatrix from node
	graphicBody->setModelMatrix(modelMatrix);
}

//nicht benötigt
void VirtualObject::translate(){

}

//nicht benötigt
void VirtualObject::scale(){

}

