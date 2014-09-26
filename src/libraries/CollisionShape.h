#ifndef COLLISION_SHAPE_H
#define COLLISION_SHAPE_H

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

//#include "Vector3.h"
#include <glm/glm.hpp>

/** \brief Collision Shape
 *
 * represents the shape of the RigidBody
 */
class CollisionShape
{
protected:
//private:
	glm::vec3 origin;	/**< origin of the shape = position of RigidBody */

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

	// <<<<<<<<<< getter + setter >>>>>>>>>> //
	const glm::vec3& getOrigin() const {
		return origin;
	}

	void setOrigin(const glm::vec3& origin) {
		this->origin = origin;
	}
};
#endif
