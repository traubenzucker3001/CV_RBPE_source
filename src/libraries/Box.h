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
	double halfsize;	/**< defines the size of the box */

public:
	Box(double halfsizeIN);

	~Box();

	void calcParticles();

	// <<<<<<<<<< getter + setter >>>>>>>>>> //
	double getHalfsize() const {
		return halfsize;
	}

	void setHalfsize(double halfsize) {
		this->halfsize = halfsize;
	}
};
#endif
