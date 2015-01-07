
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

using namespace std;

//git branch test
//link fix try 4
extern World* world;
extern Demo* demo;
extern Cuda* cuda;

VirtualObject::VirtualObject(glm::vec3 posIN, int bodyCount, float massIN, bool staticIN, bool shapeIN, float sizeIN, int idIN){

	id = idIN;
	position = posIN;
	physicBody = new RigidBody(massIN, staticIN, shapeIN, posIN,sizeIN);

	world->allBodies[bodyCount] = physicBody;

	graphicBody = new CVK::Node();

	graphicBody->setGeometry(demo->cubeGeometry);
	graphicBody->setMaterial((demo->cubeMaterial));

	graphicBody->setModelMatrix(glm::translate(glm::mat4(1.0f), position));
	demo->sceneRoot->addChild(graphicBody);
}

VirtualObject::~VirtualObject(){

	delete physicBody;
	delete graphicBody;
}

//nicht über cuda - opengl austausch
void VirtualObject::updateCPU(){

	//update modelmatrix with new rb values
	glm::vec3 tempP = world->allBodies[id]->getPosition();
	glm::quat tempQ = world->allBodies[id]->getRotationQuat();
	
	//modelmatrix neu berechnen
	glm::mat4 t = glm::translate(glm::mat4(1.0f), tempP);
	
	//!!rotations matrix umwandeln (mat3 zu mat4)!! oder quat zu mat4
	glm::mat4 r = glm::toMat4(tempQ);

	modelMatrix = t * r;

	//set new modelmatrix from node
	graphicBody->setModelMatrix(modelMatrix);
}

void VirtualObject::updateGPU(){

	//werte von gpu beschaffen
	glm::vec3 tempP = cuda->h_uVOpos[id];
	
	glm::quat tempQ = cuda->h_uVOrot[id];

	//modelmatrix neu berechnen
	glm::mat4 t = glm::translate(glm::mat4(1.0f), tempP);

	glm::mat4 r = glm::toMat4(tempQ);

	modelMatrix = t * r;

	//set new modelmatrix from node
	graphicBody->setModelMatrix(modelMatrix);
}
