#ifndef SPHERE_H
#define SPHERE_H

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "RigidBody.h"
#include "CollisionShape.h"

/** \brief Box
 *
 * CollisionShape in sphere form
 */
class Sphere : public RigidBody, public CollisionShape
{
private:
	double radius;	/**< defines the size of the sphere */

};
#endif
