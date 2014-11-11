
// <<<<<<<<<< includes >>>>>>>>>> //
#include <iostream>
#include "CollisionShape.h"

using namespace std;

CollisionShape::CollisionShape(glm::vec3 origIN, float massIN, Particle** bPartIN, int numPartIN){

	cout << "collshape: collision shape constr called!" << endl; //zum test

	origin = origIN;
	mass = massIN;
	bodyParticles = bPartIN;
	numOfPart = numPartIN;
}

CollisionShape::~CollisionShape(){

	//...
}
