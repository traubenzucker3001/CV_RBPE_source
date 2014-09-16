#ifndef FORCE_REG_ENTRY_H
#define FORCE_REG_ENTRY_H

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "RigidBody.h"
#include "ForceGen.h"

/** \brief
 *
 * ...
 */
class ForceRegEntry
{
private:
	RigidBody *body;	/**< one RigidBody */

	ForceGen *forces;	/**< list of forces that appear on the RigidBody */

public:
	/** \brief constructor
	 *
	 * ...
	 */
	ForceRegEntry(RigidBody *bodyIN, ForceGen *forceGen);

	/** \brief destructor
	 *
	 * ...
	 */
	~ForceRegEntry();
};
#endif
