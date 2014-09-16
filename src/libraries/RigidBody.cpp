#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "RigidBody.h"


void RigidBody::calcInternData(){

}

void RigidBody::integrate(){

}

void RigidBody::clearAccu(){

}

void RigidBody::addForce(glm::vec3 forceIN){

	forceToBeApplied = forceToBeApplied + forceIN;
}
