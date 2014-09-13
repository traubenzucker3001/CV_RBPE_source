#ifndef SPHERE_H
#define SPHERE_H

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "RigidBody.h"
#include "CollisionShape.h"

class Sphere : public RigidBody, public CollisionShape
{
private:
	double radius;

};
#endif
