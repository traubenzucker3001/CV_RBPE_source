#ifndef RIGID_BODY_H
#define RIGID_BODY_H

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "Vector3.h"
#include "Quaternion.h"
#include "Matrix3.h"
#include "CollisionShape.h"
#include "Matrix4.h"

class RigidBody
{
private:
	Vector3 position;

	Quaternion orientation;

	Vector3 velocity;

	Vector3 rotation;

	double mass;

	Matrix3 inertiaTensor;

	CollisionShape shape;

	double linearDamp;

	double angularDamp;

	Matrix4 transformMatrix;

	Vector3 forceToBeApplied;

	Vector3 torgueToBeApplied;

	Vector3 acceleration;

	Vector3 lastFrameAcc;

	double inverseMass;

	Matrix3 inverseInertiaTensor;

	bool isStatic;


public:
	void calcInternData();

	void integrate();

};
#endif
