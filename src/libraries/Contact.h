#ifndef CONTACT_H
#define CONTACT_H

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "RigidBody.h"
#include "Vector3.h"
#include "Matrix3.h"

class Contact
{
private:
	RigidBody body1;

	RigidBody body2;

	double friction;

	double restitution;

	Vector3 contactPoint;

	Vector3 contactNormal;

	double penetration;

	Matrix3 contactToWorld;

	Vector3 contactVelocity;

	double desiredVelocity;


public:
	void setBodyData();

	void calcInternData();

	void swapBodies();

	void calcdesiredVeloc();

	void calcLocalVeloc();

	void calcContactBasis();

	void applyImpulse();

	void applyVelocChange();

	void applyPosChange();

};
#endif
