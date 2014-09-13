#ifndef VIRTUAL_OBJECT_H
#define VIRTUAL_OBJECT_H

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "RigidBody.h"

class VirtualObject
{
private:
	RigidBody physicBody;


public:
	void update();

};
#endif
