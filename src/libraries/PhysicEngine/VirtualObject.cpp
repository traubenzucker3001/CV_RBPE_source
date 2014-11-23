
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

//git branch test

using namespace std;

VirtualObject::VirtualObject(glm::vec3 posIN, int bodyCount, float massIN, bool staticIN, bool shapeIN){

	cout << "vo: vo constr called!" << endl; //zum test

	physicBody = new RigidBody(massIN, staticIN, shapeIN, posIN);

	//schauen ob body/part world arrays richtig bzw. �berhaupt bef�llt werden!!!
	World::getInstance()->allBodies[bodyCount] = physicBody;

	graphicBody = new CVK::Node();

	graphicBody->setGeometry(Demo::getInstance()->geometry);
	//graphicBody->setMaterial();
	graphicBody->setModelMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0.72, 0)));;
	Demo::getInstance()->sceneRoot->addChild(graphicBody);
}

/*
VirtualObject::VirtualObject(bool shape, float shapeSize){

	physicBody = ;
}
*/

VirtualObject::~VirtualObject(){

	delete physicBody;
	delete graphicBody;
}

//�ber cuda - opengl austausch
void VirtualObject::updateCPU(){

	cout << "vo: update!" << endl; //zum test

	//...
	//update modelmatrix with new rb values
	RigidBody *tempB = World::getInstance()->allBodies[id];
	glm::vec3 tempP = tempB->getPosition();
	glm::mat3 tempO = tempB->getRotationMat();
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
	glm::vec3 tempP;
	glm::mat3 tempO;
	glm::quat tempQ;
	//...
	//cudaMemcpyFromSymbol
	//modelmatrix neu berechnen
	glm::mat4 t = glm::translate(glm::mat4(1.0f), tempP);

	//!!rotations matrix umwandeln (mat3 zu mat4)!! oder quat zu mat4
	//glm::mat4 r = glm::mat4(tempO, 0.0f);
	glm::mat4 r = glm::toMat4(tempQ);			//mal schaun ob so funktioniert!!?

	modelMatrix = t * r;

	//set new modelmatrix from node
	graphicBody->setModelMatrix(modelMatrix);
}

//nicht ben�tigt
void VirtualObject::translate(){

}

//nicht ben�tigt
void VirtualObject::scale(){

}

