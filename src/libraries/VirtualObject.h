#ifndef VIRTUAL_OBJECT_H
#define VIRTUAL_OBJECT_H

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
class VirtualObject
{
private:
	RigidBody physicBody;	/**< RigidBody as physics component of this object */


public:
	/** \brief
	 *
	 * ...
	 */
	void update();

};
#endif
