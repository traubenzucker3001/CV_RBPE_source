
#ifndef COLLISIONSHAPE2_H_
#define COLLISIONSHAPE2_H_

// <<<<<<<<<< includes >>>>>>>>>> //
#include <vector>

#include "Particle2.h"

class CollisionShape {

protected:

	glm::vec3 origin;
	float mass;

	//std::vector<Particle*> bodyParticles;	//besser ist array
	Particle* bodyParticles;	//**
	int numOfPart;

public:

	CollisionShape(glm::vec3 origIN, float massIN, Particle* bPartIN, float numPartIN);	//bodyPart noch dazu?!
	virtual ~CollisionShape();

	virtual void calcParticles() = 0;		//createParticles

	virtual void applyRotationToParticles() = 0;	//von wo aufgerufen vorher updateRotationMatrix();

	virtual void populateParticleArray() = 0;	//virtual?!

	// <<<<<<<<<< getter + setter >>>>>>>>>> //

};

#endif /* COLLISIONSHAPE2_H_ */
