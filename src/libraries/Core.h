#ifndef CORE_H
#define CORE_H

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "World.h"

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
