
// <<<<<<<<<< includes >>>>>>>>>> //
#include "RigidBody2.h"
#include "DemoApp/Demo2.h"

int RigidBody::count = 0;

RigidBody::RigidBody(float massIN, bool staticIN, bool shapeIN, glm::vec3 posIN){

	mass = massIN;
	inverseMass = ((float)1.0)/mass;

	position = posIN;

	isStatic = staticIN;
	if(shapeIN == true){

		shape = new Sphere();
	}
	else{

		shape = new Box();
	}

	float temp1 = Demo::getInstance()->getTerminalVeloc();
	terminalMom = temp1 * mass;

	count++;
}

RigidBody::~RigidBody(){

	//...
}

void RigidBody::iterate(float duration){


}

void RigidBody::updateRotationMatrix(){


}

void RigidBody::updateInverseInertiaTensor(){


}

void RigidBody::updatePartValues(){

	//runter in collshape greifen
}

void RigidBody::updateMomenta(float duration){

	//runter in collshape greifen
}

void RigidBody::reset(float newPosition){


}
