#ifndef FORCE_REGISTRY_H
#define FORCE_REGISTRY_H

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "ForceRegEntry.h"

class ForceRegistry
{
private:
	ForceRegEntry register;


public:
	void add();

	void remove();

	void clear();

	void updateForces();

};
#endif
