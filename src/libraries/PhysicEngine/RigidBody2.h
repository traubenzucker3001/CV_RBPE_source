
#ifndef RIGIDBODY2_H_
#define RIGIDBODY2_H_

// <<<<<<<<<< includes >>>>>>>>>> //
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include <vector>

class RigidBody {

private:

	float mass;//
	float inverseMass;	//ka ob n�tig

	glm::vec3 position;//
	glm::vec3 velocity;//
	glm::vec3 lastFrameVeloc;	//ka ob n�tig

	glm::quat rotationquaternion;//
	glm::mat3 rotationMatrix;//

	//float initialInverseInertiaTensorDiagonal[3];//
	glm::mat3 inertiaTensor;
	glm::mat3 inverseInertiaTensor;	//world oder local coords?!

	//
	glm::vec3 angularVelocity;	//rotation
	glm::vec3 angularMomentum;

	//
	glm::vec3 linearMomentum;
	float terminalMom;		//?

	glm::vec3 force;		//ehem. forcetobeapplied
	//glm::vec3 torgueToBeApplied;	//ka ob n�tig

	//glm::vec3 lastFrameVeloc;	//ka ob n�tig

	bool isStatic;

	//sp�ter drum k�mmern
	//float linearDamp;
	//float angularDamp;

	glm::mat4 transformMatrix;

	CollisionShape *shape;
	//partikel erst in collshape
	//std:vector<Particle*> bodyParticles;
	//int numberOfParticles;

	static int count;

public:

	RigidBody(float massIN, bool staticIN, bool shapeIN, glm::vec3 posIN);
	~RigidBody();

	void iterate(float duration);		//performStep, mehrere schritte zusammenfassen

	void updateRotationMatrix();
	void updateInverseInertiaTensor();

	void updatePartValues();	//runter in collshape greifen
	void updateMomenta(float duration); //runter in collshape greifen und schauen ob gravity klasse noch n�tig

	void reset(float newPosition);
};

#endif /* RIGIDBODY2_H_ */
