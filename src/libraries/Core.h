#ifndef CORE_H
#define CORE_H

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "World.h"

/** \brief Engine Core
 *
 * initial class. every "important" configs can be done/set here
 */
class Core
{
private:
	World physicsWorld;

	double velocityIterations;

	double velocityEpsilon;

	double positionIterations;

	double positionEpsilon;

	bool calcIterations;

	unsigned int maxContacts;


public:
	void createEngine();

};
#endif
