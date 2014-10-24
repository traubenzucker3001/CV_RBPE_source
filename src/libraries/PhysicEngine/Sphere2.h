
#ifndef SPHERE2_H_
#define SPHERE2_H_

#include "CollisionShape2.h"

class Sphere : public CollisionShape {

private:

	float radius;	/**< defines the size of the sphere */

public:

	Sphere(glm::vec3 origIN,  float radiusIN) : CollisionShape(origIN){radius = radiusIN;}
	virtual ~Sphere();

	void calcParticles();
	void applyRotationToParticles(glm::mat3 rotMatrix);

	//void populateParticleArray();	//virtual?!

	// <<<<<<<<<< getter + setter >>>>>>>>>> //
};

#endif /* SPHERE2_H_ */
