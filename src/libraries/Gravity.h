#ifndef GRAVITY_H
#define GRAVITY_H

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "ForceGen.h"
#include "Vector3.h"

class Gravity : public ForceGen
{
private:
	Vector3 gravity;


public:
	void updateForce();

};
#endif
