#ifndef COLLISION_SHAPE_H
#define COLLISION_SHAPE_H

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include <glm/glm.hpp>

//#include "Vector3.h"

/** \brief Collision Shape
 *
 * represents the shape of the RigidBody
 */
class CollisionShape
{
private:
	glm::vec3 origin;	/**< origin of the shape = position of RigidBody */

};
#endif
