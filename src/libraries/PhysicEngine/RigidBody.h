
#ifndef RIGIDBODY_H_
#define RIGIDBODY_H_

// <<<<<<<<<< includes >>>>>>>>>> //
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include <vector>

#include "CollisionShape.h"
#include "Box.h"
#include "Sphere.h"

/** \brief RigidBody
 *
 * ...
 */
class RigidBody {

private:

	float mass;//							/**< body mass */
	//ka ob nötig
	float inverseMass;						/**< body inverse mass */

	glm::vec3 position;//					/**< body position */
	glm::vec3 velocity;//					/**< body velocity */
	//ka ob nötig
	glm::vec3 lastFrameVeloc;				/**< body last frame velocity */

	glm::quat rotationQuat;//				/**< body rotation quaternion */
	glm::mat3 rotationMat;//				/**< body rotation matrix */

	//float initialInverseInertiaTensorDiagonal[3];//
	glm::mat3 inertiaTensor;				/**< body inertia tensor */
	glm::vec3 initInverseInertTensDiagon;	/**< initial inverse inertia tensor diagonal */
	//world oder local coords?!
	glm::mat3 inverseInertiaTensor;			/**< inverse inertia tensor */

	//
	//rotation
	glm::vec3 angularVelocity;				/**< angular velocity */
	glm::vec3 angularMomentum;				/**< angular momentum */

	//
	glm::vec3 linearMomentum;				/**< linear momentum */
	//?
	float terminalMom;						/**< terminat momentum */

	//ehem. forcetobeapplied
	glm::vec3 force;						/**< body force */

	//glm::vec3 torgueToBeApplied;	//ka ob nötig
	//glm::vec3 lastFrameVeloc;	//ka ob nötig

	bool isStatic;							/**< true if object is static, false if object is dynamic */

	//später drum kümmern
	//float linearDamp;		//welt oder jeder body einzeln?!
	//float angularDamp;

	glm::mat4 transformMatrix;				/**< transformation matrix */

	CollisionShape *shape;					/**< body shape */
	//partikel erst in collshape
	//std:vector<Particle*> bodyParticles;
	//int numberOfParticles;

	static int count;						/**< help variable for all bodies array/vector */

public:

	/** \brief constructor
	 *
	 * ...
	 */
	RigidBody(float massIN, bool staticIN, bool shapeIN, glm::vec3 posIN);

	/** \brief destructor
	 *
	 * ...
	 */
	~RigidBody();

	/** \brief iterate
	 *
	 * ...
	 */
	void iterate(float duration);		//performStep, mehrere schritte zusammenfassen

	/** \brief update rotations matrix
	 *
	 * ...
	 */
	void updateRotMatrix();

	/** \brief update inverse inertia tensor
	 *
	 * ...
	 */
	void updateInverseInertiaTensor();

	/** \brief update particle values
	 *
	 * ...
	 */
	void updatePartValues();	//runter in collshape greifen

	/** \brief update momenta
	 *
	 * ...
	 */
	void updateMomenta(float duration); //runter in collshape greifen und schauen ob gravity klasse noch nötig

	/** \brief reset body
	 *
	 * ...
	 */
	void reset(float newPosition);


	// <<<<<<<<<< gpu parts >>>>>>>>>> //
	/** \brief update cuda array
	 *
	 * ...
	 */
	void updateCUDArray(int bodyIndex);
};

#endif /* RIGIDBODY_H_ */
