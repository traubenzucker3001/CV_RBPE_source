
#include "Particle.h"

Particle::Particle(glm::vec3 pos, float mass){

}

void Particle::updateVelocity(glm::vec3 bodyPosition, glm::vec3 bodyVelocity, glm::vec3 bodyAngularVelocity) {
	velocity = {0.0f, 0.0f, 0.0f};

	float scalar = sqrt(bodyAngularVelocity.x*bodyAngularVelocity.x +
				bodyAngularVelocity.y*bodyAngularVelocity.y +
				bodyAngularVelocity.z*bodyAngularVelocity.z);

	if (scalar > 0.0f) {
		scalar *= scalar;
		glm::vec3 relativePosition = glm::vec3(position.x-bodyPosition.x,position.y-bodyPosition.y,position.z-bodyPosition.z);

		scalar = (bodyAngularVelocity.x*relativePosition.x + bodyAngularVelocity.y*relativePosition.y + bodyAngularVelocity.z*relativePosition.z) / scalar;

		glm::vec3 term;
		term.x = relativePosition.x - bodyAngularVelocity.x*scalar;
		term.y = relativePosition.y - bodyAngularVelocity.y*scalar;
		term.z = relativePosition.z - bodyAngularVelocity.z*scalar;

		//
		velocity.x = (bodyAngularVelocity.y*term.z - bodyAngularVelocity.z*term.y);
		velocity.y = (bodyAngularVelocity.z*term.x - bodyAngularVelocity.x*term.z);
		velocity.z = (bodyAngularVelocity.x*term.y - bodyAngularVelocity.y*term.x);
	}
	velocity.x += bodyVelocity.x;
	velocity.y += bodyVelocity.y;
	velocity.z += bodyVelocity.z;
}

void Particle::applyRotation(glm::mat3 rotationMatrix, glm::vec3 originalRelativePos, glm::vec3 bodyPosition) {

	position.x = originalRelativePos.x*rotationMatrix[0].x +
		originalRelativePos.y*rotationMatrix[0].y +
		originalRelativePos.z*rotationMatrix[0].z;

	position.y = originalRelativePos.x*rotationMatrix[1].x +
		originalRelativePos.y*rotationMatrix[1].y +
		originalRelativePos.z*rotationMatrix[1].z;

	position.z = originalRelativePos.x*rotationMatrix[2].x +
		originalRelativePos.y*rotationMatrix[2].y +
		originalRelativePos.z*rotationMatrix[2].z;

	position.x = position.x + bodyPosition.x;
	position.y = position.y + bodyPosition.y;
	position.z = position.z + bodyPosition.z;
}
