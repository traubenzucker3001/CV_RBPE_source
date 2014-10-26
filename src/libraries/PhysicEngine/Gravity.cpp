/*
#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "Gravity.h"
#include "RigidBody.h"

Gravity::Gravity(const glm::vec3 &gravIN){
	gravity = gravIN;
}

void Gravity::updateForce(RigidBody* body, float duration){

	//wenn nicht static
	if (body->isIsStatic() == false){
		//kraft zu body hinzufügen
		body->addForce(gravity * body->getMass());
	}
	return;

}
*/
