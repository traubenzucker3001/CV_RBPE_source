#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "VirtualObject.h"


	VirtualObject::VirtualObject(RigidBody* physBody){

		physicBody = physBody;
	}

	/*
	VirtualObject::VirtualObject(bool shape, float shapeSize){

		physicBody = ;
	}
	*/

	VirtualObject::~VirtualObject(){

	}

	void VirtualObject::update(){

	}

	void VirtualObject::translate(){

	}

	void VirtualObject::scale(){

	}

