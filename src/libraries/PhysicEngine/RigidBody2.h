
#ifndef RIGIDBODY2_H_
#define RIGIDBODY2_H_

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include <vector>

class RigidBody {

private:

	float mass;//
	//float inverseMass;	//ka ob nötig

	glm::vec3 position;//
	glm::vec3 velocity;//
	glm::vec3 lastFrameVeloc;	//ka ob nötig

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
	float terminalMomentum;		//?

	glm::vec3 force;		//ehem. forcetobeapplied
	//glm::vec3 torgueToBeApplied;	//ka ob nötig

	//glm::vec3 lastFrameVeloc;	//ka ob nötig

	bool isStatic;

	float linearDamp;
	float angularDamp;

	glm::mat4 transformMatrix;

	CollisionShape *shape;
	//partikel erst in collshape
	//std:vector<Particle*> bodyParticles;
	//int numberOfParticles;

public:

	RigidBody();
	~RigidBody();

	void iterate(float duration);		//performStep

	void updateRotationMatrix();
	void updateInverseInertiaTensor() {};

	void updateParticleValues();	//runter in collshape greifen
	void updateMomenta(float duration); //runter in collshape greifen

	void reset(float newPosition);
};

#endif /* RIGIDBODY2_H_ */
