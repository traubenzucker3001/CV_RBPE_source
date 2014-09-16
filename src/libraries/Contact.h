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
	RigidBody *body1;			/**< involved body 1 */

	RigidBody *body2;			/**< involved body 2 */

	double friction;			/**< friction coefficient */

	double restitution;			/**< restitution coefficient */

	glm::vec3 contactPoint;		/**< contact position */

	glm::vec3 contactNormal;	/**< contact normal */

	double penetration;			/**< penetration */

	glm::mat3 contactToWorld;	/**< conversion help */

	glm::vec3 contactVelocity;	/**< contact velocity */

	double desiredVelocity;		/**< desired velocity to resolve contact */


public:
	/** \brief set contact data
	 *
	 * set data that does not depend on contact position
	 */
	void setBodyData(RigidBody *oneIN, RigidBody *twoIN, double fricIN, double restiIN);

	/** \brief
	 *
	 * ...
	 */
	void calcInternData(double duration);

	/** \brief swap RigidBodies
	 *
	 * swaps the bodies and changes the corresponding data
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
