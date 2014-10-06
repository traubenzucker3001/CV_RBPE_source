#ifndef FORCE_REGISTRY_H
#define FORCE_REGISTRY_H

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "ForceRegEntry.h"


/** \brief
 *
 * ...
 */
class ForceRegistry
{
private:
	std::vector<ForceRegEntry> regist;		/**< list of entries */

public:
	/** \brief add entry
	 *
	 * adds new entry to list
	 */
	void add(RigidBody *body, ForceGen *forceGen);

	/** \brief remove entry
	 *
	 * removes an entry from list
	 */
	void remove(RigidBody *body, ForceGen *forceGen);

	/** \brief clear list
	 *
	 * clears the hole list
	 */
	void clear();

	/** \brief
	 *
	 * ...
	 */
	void updateForces(float duration);

};
#endif
