#ifndef FORCE_GEN_H
#define FORCE_GEN_H

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

/** \brief
 *
 * ...
 */
class ForceGen
{
public:
	/** \brief update
	 *
	 * apply force to given rigidBody
	 */
	virtual void updateForce(RigidBody *body, double duration);

	/** \brief destructor
	 *
	 * ...
	 */
	virtual ~ForceGen();

};
#endif
