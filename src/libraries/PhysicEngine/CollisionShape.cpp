
// <<<<<<<<<< includes >>>>>>>>>> //
#include <iostream>
#include "CollisionShape.h"

using namespace std;

CollisionShape::CollisionShape(glm::vec3 origIN, float massIN, int numPartIN){

	origin = origIN;
	mass = massIN;
	bodyParticles = 0;
	numOfPart = numPartIN;
}

CollisionShape::~CollisionShape(){

	delete bodyParticles;
}
