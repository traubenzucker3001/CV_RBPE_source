#ifndef RIGID_BODY_H
#define RIGID_BODY_H

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

//#include "Vector3.h"
//#include "Quaternion.h"
//#include "Matrix3.h"
#include "CollisionShape.h"
//#include "Matrix4.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

/** \brief
 *
 * ...
 */
class RigidBody
{
private:
	glm::vec3 position;				/**< ... */

	glm::quat orientation;			/**< ... */

	glm::vec3 velocity;				/**< ... */

	glm::vec3 rotation;				/**< ... */

	double mass;					/**< ... */

	glm::mat3 inertiaTensor;			/**< ... */

	CollisionShape shape;			/**< ... */

	double linearDamp;				/**< ... */

	double angularDamp;				/**< ... */

	glm::mat4 transformMatrix;		/**< ... */

	glm::vec3 forceToBeApplied;		/**< ... */

	glm::vec3 torgueToBeApplied;	/**< ... */

	glm::vec3 acceleration;			/**< ... */

	glm::vec3 lastFrameAcc;			/**< ... */

	double inverseMass;				/**< ... */

	glm::vec3 inverseInertiaTensor;	/**< ... */

	bool isStatic;					/**< ... */


public:
	/** \brief
	 *
	 * ...
	 */
	void calcInternData();

	/** \brief
	 *
	 * ...
	 */
	void integrate();

};
#endif
