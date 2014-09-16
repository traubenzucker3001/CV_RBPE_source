#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "ForceRegistry.h"


void ForceRegistry::add(RigidBody *body, ForceGen *forceGen){

	ForceRegEntry newEntry = ForceRegEntry(body, forceGen);
	regist.push_back(newEntry);
}

void ForceRegistry::remove(RigidBody *body, ForceGen *forceGen){


}

void ForceRegistry::clear(){


}

void ForceRegistry::updateForces(double duration){


}
