
// <<<<<<<<<< includes >>>>>>>>>> //
#include "CollisionShape2.h"

CollisionShape::CollisionShape(glm::vec3 origIN, float massIN, Particle* bPartIN, float numPartIN){

	origin = origIN;
	mass = massIN;
	bodyParticles = bPartIN;
	numOfPart = numPartIN;
}

CollisionShape::~CollisionShape(){

	//...
}
