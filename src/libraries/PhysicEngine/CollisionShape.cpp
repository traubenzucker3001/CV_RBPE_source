
// <<<<<<<<<< includes >>>>>>>>>> //
#include "CollisionShape.h"

CollisionShape::CollisionShape(glm::vec3 origIN, float massIN, Particle** bPartIN, int numPartIN){

	origin = origIN;
	mass = massIN;
	bodyParticles = bPartIN;
	numOfPart = numPartIN;
}

CollisionShape::~CollisionShape(){

	//...
}
