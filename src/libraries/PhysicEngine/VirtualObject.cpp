#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "VirtualObject.h"

using namespace std;

VirtualObject::VirtualObject(float sizeIN, glm::vec3 posIN){

	cout << "vo: vo constr called!" << endl; //zum test

	physicBody = new RigidBody(sizeIN);
	graphicBody = new CVK::Cube(sizeIN);
}

/*
VirtualObject::VirtualObject(bool shape, float shapeSize){

	physicBody = ;
}
*/

VirtualObject::~VirtualObject(){

	delete physicBody;
	delete graphicBody;
}

//�ber cuda - opengl austausch
void VirtualObject::update(){

	cout << "vo: update!" << endl; //zum test

	//...
}

//nicht ben�tigt
void VirtualObject::translate(){

}

//nicht ben�tigt
void VirtualObject::scale(){

}

