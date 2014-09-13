#ifndef FORCE_REG_ENTRY_H
#define FORCE_REG_ENTRY_H

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "RigidBody.h"
#include "ForceGen.h"

class ForceRegEntry
{
private:
	RigidBody body;

	ForceGen forces;

};
#endif
