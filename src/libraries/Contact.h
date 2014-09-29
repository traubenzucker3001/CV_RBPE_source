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
class Contact{

private:
	RigidBody *collBodies[2];	/**< involved bodies */

	double friction;			/**< friction coefficient */

	double restitution;			/**< restitution coefficient */

	glm::vec3 contactPoint;		/**< contact position */

	glm::vec3 contactNormal;	/**< contact normal */

	double penetration;			/**< penetration */

	glm::mat3 contactToWorld;	/**< conversion help */

	glm::vec3 contactVelocity;	/**< contact velocity */

	double desiredVelocity;		/**< desired velocity to resolve contact */

	glm::vec3 relatContPos[2];	/**< position of contact point in world space relative to center of body1 */

public:
	/** \brief set contact data
	 *
	 * set data that does not depend on contact position
	 */
	void setBodyData(RigidBody *oneIN, RigidBody *twoIN, double fricIN, double restiIN);

	/** \brief calculate intern data
	 *
	 * calculate relative-contact-position and contact velocity
	 */
	void calcInternData(double duration);

	/** \brief swap RigidBodies
	 *
	 * swaps the bodies and changes the corresponding data
	 */
	void swapBodies();

	/** \brief calculate derived velocity
	 *
	 * calculate necessary velocity to resolve the contact
	 */
	void calcDesiredVeloc(double duration);

	/** \brief calculate local velocity
	 *
	 * calculate velocity at contact point from given body
	 */
	glm::vec3 calcLocalVeloc(unsigned int bodyIndex, double duration);

	/** \brief calculate contact basis
	 *
	 * calculate an orthonormal basis for the contact point
	 */
	void calcContactBasis();

	/** \brief
	 *
	 * ...
	 */
	void applyImpulse();

	/** \brief apply velocity change
	 *
	 * impulse based contact resolution
	 */
	void applyVelocChange(glm::vec3 velocChange[2], glm::vec3 rotatChange[2]);

	/** \brief apply position change
	 *
	 * penetration resolution of the contact
	 */
	void applyPosChange(glm::vec3 velocChange[2], glm::vec3 rotatDirection[2], double rotatAmount[2], double penetration);


	// <<<<<<<<<< getter + setter >>>>>>>>>> //
	double getDesiredVelocity() const {
		return desiredVelocity;
	}

	void setDesiredVelocity(double desiredVelocity) {
		this->desiredVelocity = desiredVelocity;
	}
};
#endif
