
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

VirtualObject::VirtualObject(glm::vec3 posIN, int bodyCount, float massIN, bool staticIN, bool shapeIN, float sizeIN){

	cout << "vo: vo constr called!" << endl; //zum test

	physicBody = new RigidBody(massIN, staticIN, shapeIN, posIN,sizeIN);

	//schauen ob body/part world arrays richtig bzw. überhaupt befüllt werden!!!
	world->allBodies[bodyCount] = physicBody;

	graphicBody = new CVK::Node();

	graphicBody->setGeometry(demo->geometry);
	//graphicBody->setMaterial();
	graphicBody->setModelMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0.72, 0)));;
	demo->sceneRoot->addChild(graphicBody);
}

VirtualObject::~VirtualObject(){

	delete physicBody;
	delete graphicBody;
}

//über cuda - opengl austausch
void VirtualObject::updateCPU(){

	cout << "vo: update!" << endl; //zum test

	//...
	//update modelmatrix with new rb values
	RigidBody *tempB = world->allBodies[id];
	glm::vec3 tempP = tempB->getPosition();
	//glm::mat3 tempO = tempB->getRotationMat();
	glm::quat tempQ = tempB->getRotationQuat();

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
	
	//TODO
	//werte von gpu beschaffen
	glm::vec3 tempP = cuda->h_uVOpos[id];
	//glm::mat3 tempO;
	glm::quat tempQ = cuda->h_uVOrot[id];
	//...
	//cudaMemcpyFromSymbol	//nicht hier, alle benötigten daten in h_array und dann daraus werte holen

	//modelmatrix neu berechnen
	glm::mat4 t = glm::translate(glm::mat4(1.0f), tempP);

	glm::mat4 r = glm::toMat4(tempQ);			//mal schaun ob so funktioniert!!?

	modelMatrix = t * r;

	//set new modelmatrix from node
	graphicBody->setModelMatrix(modelMatrix);
}

//nicht benötigt
void VirtualObject::translate(){

}

//nicht benötigt
void VirtualObject::scale(){

}

