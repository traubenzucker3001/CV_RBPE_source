#ifndef COLLISION_SHAPE_H
#define COLLISION_SHAPE_H

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

//#include "Vector3.h"
#include <glm/glm.hpp>
//#include "Sphere.h"
//#include "Box.h"

//!überlegen ob nicht raus lassen und sphere/box direkt von body erben!

/** \brief Collision Shape
 *
 * represents the shape of the RigidBody
 */
class CollisionShape
{
protected:
//private:
	glm::vec3 origin;	/**< origin of the shape = position of RigidBody */

	//+
	std::vector<Particle*> bodyParticle;

	int bPartNum;

	float mass;

public:
	/** \brief constructor
	 *
	 * ...
	 */
	CollisionShape(glm::vec3 origIN);

	/** \brief destructor
	 *
	 * ...
	 */
	virtual ~CollisionShape();

	virtual void calcParticles() = 0;

	virtual void applyRotationToParticles() = 0;

	// <<<<<<<<<< getter + setter >>>>>>>>>> //
	const glm::vec3& getOrigin() const {
		return origin;
	}

	void setOrigin(const glm::vec3& origin) {
		this->origin = origin;
	}

	const std::vector<Particle*>& getBodyParticle() const {
		return bodyParticle;
	}

	void setBodyParticle(const std::vector<Particle*>& bodyParticle) {
		this->bodyParticle = bodyParticle;
	}
};
#endif
