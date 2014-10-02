#ifndef SPHERE_H
#define SPHERE_H

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

//#include "RigidBody.h"
#include "CollisionShape.h"

/** \brief sphere
 *
 * CollisionShape in sphere form
 */
class Sphere : public CollisionShape		//noch mal schaun ob vererbung korrekt!!
{
private:
	double radius;	/**< defines the size of the sphere */

public:
	Sphere(glm::vec3 origIN,  double radiusIN) : CollisionShape(origIN){radius = radiusIN;}

	~Sphere();

	void calcParticles();

	// <<<<<<<<<< getter + setter >>>>>>>>>> //
	double getRadius() const {
		return radius;
	}

	void setRadius(double radius) {
		this->radius = radius;
	}
};
#endif
