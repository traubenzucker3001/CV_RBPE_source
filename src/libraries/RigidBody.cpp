#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "RigidBody.h"

RigidBody::RigidBody(double massIN, bool staticIN, double linDampIN, double angDampIN, bool shapeIN){

	isStatic = staticIN;
	linearDamp = linDampIN;
	angularDamp = angDampIN;
	mass = massIN;
	inverseMass = ((double)1.0)/mass;
	//box oder sphere collision shape
	//true => kugel
	if(shapeIN == true){
		shape = new Sphere();
	}
	//false => box
	else{
		shape = new Box();
	}
	//neuen body an world hängen
	World::getInstance()->addBody(this);
}

RigidBody::~RigidBody(){

	delete position;
	delete orientation;
	delete velocity;
	delete rotation;
	delete mass;
	delete inertiaTensor;
	delete shape;
	delete linearDamp;
	delete angularDamp;
	delete transformMatrix;
	delete forceToBeApplied;
	delete torgueToBeApplied;
	delete acceleration;
	delete lastFrameVeloc;
	delete inverseMass;
	delete inverseInertiaTensorL;
	delete inverseInertiaTensorW;
	delete isStatic;
}

void RigidBody::calcInternData(){

	//orientation.normalise();
	glm::normalize(orientation);

	//Calculate the transform matrix for the body.
	calcTransMat(transformMatrix, position, orientation);

	//Calculate the inertiaTensor in world space.
	transInertiaTensor(inverseInertiaTensorW, orientation, inertiaTensor, transformMatrix);
}

void RigidBody::integrate(double duration){

	//Calculate linear acceleration from force inputs.
	lastFrameVeloc = acceleration;

	//lastFrameAcc.addScaledVector(forceToBeApplied, inverseMass);
	lastFrameVeloc.x = lastFrameVeloc.x + (forceToBeApplied.x * inverseMass);
	lastFrameVeloc.y = lastFrameVeloc.y + (forceToBeApplied.y * inverseMass);
	lastFrameVeloc.z = lastFrameVeloc.z + (forceToBeApplied.z * inverseMass);

	//calculate angular acceleration from torque inputs.
	//Vector3 angularAcceleration = inverseInertiaTensorWorld.transform(torqueAccum);
	glm::vec3 angAcc = inverseInertiaTensorW * torgueToBeApplied;

	//Adjust velocities
	//Update linear velocity from both acceleration and impulse.
	//velocity.addScaledVector(lastFrameAcceleration, duration);
	velocity.x = velocity.x + (lastFrameVeloc.x * duration);
	velocity.y = velocity.y + (lastFrameVeloc.y * duration);
	velocity.z = velocity.z + (lastFrameVeloc.z * duration);

	// Update angular velocity from both acceleration and impulse.
	//rotation.addScaledVector(angularAcceleration, duration);
	rotation.x = rotation.x + (angAcc.x * duration);
	rotation.y = rotation.y + (angAcc.y * duration);
	rotation.z = rotation.z + (angAcc.z * duration);

	// Impose drag.
	velocity = velocity * pow(linearDamp, duration);
	rotation = rotation * pow(angularDamp, duration);

	// Adjust positions
	// Update linear position.
	//position.addScaledVector(velocity, duration);
	position.x = position.x + (velocity.x * duration);
	position.y = position.y + (velocity.y * duration);
	position.z = position.z + (velocity.z * duration);

	// Update angular position.
	//orientation.addScaledVector(rotation, duration);
	//...TODO...

	// Impose drag.
	//doppelt!!?
	velocity = velocity * pow(linearDamp, duration);
	rotation = rotation * pow(angularDamp, duration);

	// Normalise the orientation, and update the matrices with the new
	// position and orientation
	calcInternData();

	// Clear accumulators.
	clearAccu();

}

void RigidBody::clearAccu(){

	forceToBeApplied.x = forceToBeApplied.y = forceToBeApplied.z = 0;
	torgueToBeApplied.x = torgueToBeApplied.y = torgueToBeApplied.z =0;
}

void RigidBody::addForce(glm::vec3 forceIN){

	forceToBeApplied = forceToBeApplied + forceIN;
}

void RigidBody::calcTransMat(glm::mat4 tmIN, glm::vec3 pIN, glm::dquat oIN){

	//TODO
}

void RigidBody::transInertiaTensor(glm::mat3 iitIN, glm::dquat oIN, glm::mat3 itIN, glm::mat4 tmIN){

	//TODO
}

void RigidBody::addVelocity(glm::vec3 velocIN){

	velocity = velocity + velocIN;
}

void RigidBody::addRotation(glm::vec3 rotatIN){

	rotation = rotation + rotatIN;
}

// <<<<<<<<<< getter + setter >>>>>>>>>> //
//siehe RigidBody.h
