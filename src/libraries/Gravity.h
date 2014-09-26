#ifndef GRAVITY_H
#define GRAVITY_H

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "ForceGen.h"
//#include "Vector3.h"
#include <glm/glm.hpp>

/** \brief
 *
 * ...
 */
class Gravity : public ForceGen
{
private:
	glm::vec3 gravity;	/**< gravity value */


public:
	/** \brief constructor
	 *
	 * ...
	 */
	Gravity(const glm::vec3 &gravity);

	~Gravity();

	/** \brief
	 *
	 * acts gravity to a RigidBody over a given duration
	 */
	void updateForce(RigidBody* body, double duration);

};
#endif
