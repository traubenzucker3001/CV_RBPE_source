#ifndef RIGID_BODY_REG_H
#define RIGID_BODY_REG_H

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "RigidBody.h"

/** \brief
 *
 * ...
 */
class RigidBodyReg
{
private:
	RigidBody *body;		/**< RigidBody */

	//strucktur umwerfen und mit std::vector regeln!?
	RigidBodyReg *next;		/**< next list entry */

public:
	/** \brief constructor
	 *
	 * ...
	 */
	RigidBodyReg(RigidBody bodyIN);

	/** \brief destructor
	 *
	 * ...
	 */
	~RigidBodyReg();

};
#endif
