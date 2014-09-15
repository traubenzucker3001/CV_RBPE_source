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
	ForceRegEntry register;		/**< list of entries */


public:
	/** \brief
	 *
	 * ...
	 */
	void add();

	/** \brief
	 *
	 * ...
	 */
	void remove();

	/** \brief
	 *
	 * ...
	 */
	void clear();

	/** \brief
	 *
	 * ...
	 */
	void updateForces();

};
#endif
