//neue Klasse - Rigidbody2.h

/*
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
/*class RigidBody
{
private:
	glm::vec3 position;					/**< position */

/*	glm::quat orientation;				/**< orientation */

/*	glm::vec3 velocity;					/**< velocity */

/*	glm::vec3 rotation;					/**< rotation */

/*	float mass;							/**< mass */

/*	glm::mat3 inertiaTensor;			/**< inertia tensor */

/*	CollisionShape *shape;				/**< shape, sphere or box */

/*	float linearDamp;					/**< linear damping */

/*	float angularDamp;					/**< angular damping */

/*	glm::mat4 transformMatrix;			/**< ... */

/*	glm::vec3 forceToBeApplied;			/**< ... */

/*	glm::vec3 torgueToBeApplied;		/**< ... */

/*	glm::vec3 acceleration;				/**< acceleration */

/*	glm::vec3 lastFrameVeloc;			/**< last frame velocity */

/*	float inverseMass;					/**< inverse mass */

/*	glm::mat3 inverseInertiaTensorL;	/**< inverse inertia tensor in body coordinates */

/*	glm::mat3 inverseInertiaTensorW;	/**< inverse inertia tensor in world coordinates. */

/*	bool isStatic;						/**< true if this is a static object */

	//+
/*	glm::mat3 rotationMatrix;
	glm::vec3 angularVelocity;
	glm::vec3 force;


public:
	/** \brief constructor
	 *
	 * ...
	 */
/*	RigidBody(glm::vec3 posIN, float massIN, bool staticIN, float linDampIN, float angDampIN, bool shapeIN, float shapeSize);

	/** \brief destructor
	 *
	 * ...
	 */
/*	~RigidBody();

	/** \brief
	 *
	 * ...
	 */
/*	void calcInternData();

	/** \brief
	 *
	 * ...
	 */
/*	void integrate(float duration);

	/** \brief
	 *
	 * ...
	 */
/*	void clearAccu();

	/** \brief
	 *
	 * ...
	 */
/*	void addForce(glm::vec3 forceIN);

	/** \brief calculate transform matrix
	 *
	 * creates a transform matrix from a position and orientation
	 */
/*	void calcTransMat(glm::mat4 tmIN, glm::vec3 pIN, glm::quat oIN);

	/** \brief transform inertia tensor
	 *
	 * transform inertia tensor by a quaternion.
	 */
/*	void transInertiaTensor(glm::mat3 iitIN, glm::quat oIN, glm::mat3 itIN, glm::mat4 tmIN);

	/** \brief add velocity
	 *
	 * ...
	 */
/*	void addVelocity(glm::vec3 velocIN);

	/** \brief add rotation
	 *
	 * ...
	 */
/*	void addRotation(glm::vec3 rotatIN);

	//+
	void updatePartValues();
	void updateRotMatrix();
	void updateMomenta(float duration);

	// <<<<<<<<<< getter + setter >>>>>>>>>> //
	bool isIsStatic() const {
		return isStatic;
	}

	void setIsStatic(bool isStatic) {
		this->isStatic = isStatic;
	}

	float getMass() const {
		return mass;
	}

	void setMass(float mass) {
		this->mass = mass;
	}

	const glm::quat& getOrientation() const {
		return orientation;
	}

	void setOrientation(const glm::quat& orientation) {
		this->orientation = orientation;
	}

	const glm::vec3& getPosition() const {
		return position;
	}

	void setPosition(const glm::vec3& position) {
		this->position = position;
	}

	const glm::vec3& getRotation() const {
		return rotation;
	}

	void setRotation(const glm::vec3& rotation) {
		this->rotation = rotation;
	}

	const glm::vec3& getVelocity() const {
		return velocity;
	}

	void setVelocity(const glm::vec3& velocity) {
		this->velocity = velocity;
	}

	const glm::vec3& getLastFrameVeloc() const {
		return lastFrameVeloc;
	}

	void setLastFrameVeloc(const glm::vec3& lastFrameVeloc) {
		this->lastFrameVeloc = lastFrameVeloc;
	}

	float getInverseMass() const {
		return inverseMass;
	}

	void setInverseMass(float inverseMass) {
		this->inverseMass = inverseMass;
	}

	const glm::mat3& getInverseInertiaTensorL() const {
		return inverseInertiaTensorL;
	}

	void setInverseInertiaTensorL(const glm::mat3& inverseInertiaTensorL) {
		this->inverseInertiaTensorL = inverseInertiaTensorL;
	}

	const glm::mat3& getInverseInertiaTensorW() const {
		return inverseInertiaTensorW;
	}

	void setInverseInertiaTensorW(const glm::mat3& inverseInertiaTensorW) {
		this->inverseInertiaTensorW = inverseInertiaTensorW;
	}
};
#endif
*/
