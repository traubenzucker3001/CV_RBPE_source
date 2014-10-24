
#ifndef COLLISIONSHAPE2_H_
#define COLLISIONSHAPE2_H_

#include <vector>

class CollisionShape {

protected:

	glm::vec3 origin;
	float mass;

	std:vector<Particle*> bodyParticles;
	int numberOfParticles;

public:

	CollisionShape(glm::vec3 origIN);
	virtual ~CollisionShape();

	virtual void calcParticles() = 0;		//createParticles

	virtual void applyRotationToParticles() = 0;	//von wo aufgerufen vorher updateRotationMatrix();

	void populateParticleArray();	//virtual?!

	// <<<<<<<<<< getter + setter >>>>>>>>>> //

};

#endif /* COLLISIONSHAPE2_H_ */
