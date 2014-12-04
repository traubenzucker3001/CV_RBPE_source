
// <<<<<<<<<< includes >>>>>>>>>> //
#include <iostream>
#include "CollisionShape.h"

using namespace std;

CollisionShape::CollisionShape(glm::vec3 origIN, float massIN, int numPartIN){

	cout << "collshape: collision shape constr called!" << endl; //zum test

	origin = origIN;
	mass = massIN;
	bodyParticles = 0;
	numOfPart = numPartIN;
}

CollisionShape::~CollisionShape(){

	delete bodyParticles;
}
