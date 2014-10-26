/*
#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "RigidBody.h"
#include "World.h"
#include "CollisionShape.h"
#include "Sphere.h"
#include "Box.h"

RigidBody::RigidBody(glm::vec3 posIN, float massIN, bool staticIN, float linDampIN, float angDampIN, bool shapeIN, float shapeSize){

	position = posIN;
	isStatic = staticIN;
	linearDamp = linDampIN;
	angularDamp = angDampIN;
	mass = massIN;
	inverseMass = ((float)1.0)/mass;
	//box oder sphere collision shape
	//true => kugel
	glm::vec3 origin = posIN;
	if(shapeIN == true){
		shape = new Sphere(origin, shapeSize, mass);
	}
	//false => box
	else{
		shape = new Box(origin, shapeSize, mass);
	}
	//neuen body an world hängen
	World::getInstance()->addBody(this);
}

RigidBody::~RigidBody(){

	/*
	delete position;
	delete orientation;
	delete velocity;
	delete rotation;
	delete mass;
	delete inertiaTensor;
	*/
/*	delete shape;
	/*
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
	*/
/*}

void RigidBody::calcInternData(){

	//orientation.normalise();
	glm::normalize(orientation);

	//Calculate the transform matrix for the body.
	calcTransMat(transformMatrix, position, orientation);

	//Calculate the inertiaTensor in world space.
	transInertiaTensor(inverseInertiaTensorW, orientation, inertiaTensor, transformMatrix);
}

void RigidBody::integrate(float duration){

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

void RigidBody::calcTransMat(glm::mat4 tmIN, glm::vec3 pIN, glm::quat oIN){

	//TODO
}

void RigidBody::transInertiaTensor(glm::mat3 iitIN, glm::quat oIN, glm::mat3 itIN, glm::mat4 tmIN){

	//TODO
}

void RigidBody::addVelocity(glm::vec3 velocIN){

	velocity = velocity + velocIN;
}

void RigidBody::addRotation(glm::vec3 rotatIN){

	rotation = rotation + rotatIN;
}


//+
void RigidBody::updatePartValues(){
	updateRotMatrix();
	shape->applyRotationToParticles();

	//Update particle velocity
	//for (int i=0; i<numberOfParticles; i++) {
	std:vector<Particle*> temp = shape->getBodyParticle();
	for(std::vector<Particle*>::iterator it = temp.begin(); it != temp.end(); ++it){
		(*it)->updateVelocity(position, velocity, angularVelocity);
	}
}

//+
void RigidBody::updateMomenta(float duration) {
	force = {0.0f, 0.0f, 0.0f}; //reset forces
	force.x = force.x + mass * -9.81f; //force of gravity

	float torque[3] = {0.0f, 0.0f, 0.0f};

	//iterator über shape->bodyparticles
	for (int i=0; i<numberOfParticles; i++) {
		float* particleForce = particles[i]->calculateForces();
		force[0] += particleForce[0];
		force[1] += particleForce[1];
		force[2] += particleForce[2];

		float* particlePos = particles[i]->getPosition();
		float relativePos[3];
		relativePos[0] = particlePos[0] - position[0];
		relativePos[1] = particlePos[1] - position[1];
		relativePos[2] = particlePos[2] - position[2];

		torque[0] += relativePos[1]*particleForce[2] - relativePos[2]*particleForce[1];
		torque[1] += relativePos[2]*particleForce[0] - relativePos[0]*particleForce[2];
		torque[2] += relativePos[0]*particleForce[1] - relativePos[1]*particleForce[0];
	}

	for (int i=0; i<3; i++) {

		//zusätzl. var für lin. momenta von body nötig
		linearMomentum[i] += force[i] * delta;
		if (linearMomentum[i] > 0.0f) {
			linearMomentum[i] = std::min(linearMomentum[i],terminalMomentum);
		} else {
			linearMomentum[i] = std::max(linearMomentum[i],-terminalMomentum);
		}

		angularMomentum[i] += torque[i] * delta;
	}
}


// <<<<<<<<<< getter + setter >>>>>>>>>> //
//siehe RigidBody.h
*/
