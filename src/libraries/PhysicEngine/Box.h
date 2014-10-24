#ifndef BOX_H
#define BOX_H

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

//#include "RigidBody.h"
#include "CollisionShape.h"

/** \brief Box
 *
 * CollisionShape in box form
 */
class Box : public CollisionShape			//noch mal schaun ob vererbung korrekt!!
{
private:
	float halfsize;	/**< defines the size of the box */

public:
	Box(glm::vec3 origIN, float halfsizeIN) : CollisionShape(origIN){halfsize = halfsizeIN;}

	virtual ~Box();

	void calcParticles();
	void applyRotatToPart(glm::mat3 rotMatrix);

	// <<<<<<<<<< getter + setter >>>>>>>>>> //
	float getHalfsize() const {
		return halfsize;
	}

	void setHalfsize(float halfsize) {
		this->halfsize = halfsize;
	}
};
#endif
