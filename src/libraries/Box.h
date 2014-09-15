#ifndef BOX_H
#define BOX_H

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "RigidBody.h"
#include "CollisionShape.h"

/** \brief Box
 *
 * test test test
 */
class Box : public RigidBody, public CollisionShape
{
private:
	double halfsize;

};
#endif
