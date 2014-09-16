#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "Contact.h"


void Contact::setBodyData(RigidBody *oneIN, RigidBody *twoIN, double fricIN, double restiIN) {

	body1 = oneIN;
	body2 = twoIN;
	friction = fricIN;
	restitution = restiIN;
}

void Contact::calcInternData(double duration){


}

void Contact::swapBodies(){

	contactNormal = contactNormal * -1;

	RigidBody *temp = body1;
	body1 = body2;
	body2 = temp;
}

void Contact::calcdesiredVeloc(){


}

void Contact::calcLocalVeloc(){


}

void Contact::calcContactBasis(){


}

void Contact::applyImpulse(){


}

void Contact::applyVelocChange(){


}

void Contact::applyPosChange(){


}
