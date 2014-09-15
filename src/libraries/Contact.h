#ifndef CONTACT_H
#define CONTACT_H

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "RigidBody.h"
//#include "Vector3.h"
//#include "Matrix3.h"
#include <glm/glm.hpp>

/** \brief
 *
 * ...
 */
class Contact
{
private:
	RigidBody body1;			/**< ... */

	RigidBody body2;			/**< ... */

	double friction;			/**< ... */

	double restitution;			/**< ... */

	glm::vec3 contactPoint;		/**< ... */

	glm::vec3 contactNormal;	/**< ... */

	double penetration;			/**< ... */

	glm::mat3 contactToWorld;	/**< ... */

	glm::vec3 contactVelocity;	/**< ... */

	double desiredVelocity;		/**< ... */


public:
	/** \brief
	 *
	 * ...
	 */
	void setBodyData();

	/** \brief
	 *
	 * ...
	 */
	void calcInternData();

	/** \brief
	 *
	 * ...
	 */
	void swapBodies();

	/** \brief
	 *
	 * ...
	 */
	void calcdesiredVeloc();

	/** \brief
	 *
	 * ...
	 */
	void calcLocalVeloc();

	/** \brief
	 *
	 * ...
	 */
	void calcContactBasis();

	/** \brief
	 *
	 * ...
	 */
	void applyImpulse();

	/** \brief
	 *
	 * ...
	 */
	void applyVelocChange();

	/** \brief
	 *
	 * ...
	 */
	void applyPosChange();

};
#endif
