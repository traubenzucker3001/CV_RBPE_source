#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "VirtualObject.h"


VirtualObject::VirtualObject(float sizeIN, glm::vec3 posIN){

	physicBody = new RigidBody(sizeIN);
	graphicBody = new CVK::Cube(sizeIN);
}

/*
VirtualObject::VirtualObject(bool shape, float shapeSize){

	physicBody = ;
}
*/

VirtualObject::~VirtualObject(){

}

//�ber cuda - opengl austausch
void VirtualObject::update(){

}

//nicht ben�tigt
void VirtualObject::translate(){

}

//nicht ben�tigt
void VirtualObject::scale(){

}

