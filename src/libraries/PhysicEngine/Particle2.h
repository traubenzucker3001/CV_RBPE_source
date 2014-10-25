/*
 * Particle2.h
 *
 *  Created on: 24.10.2014
 *      Author: Daniel
 */

#ifndef PARTICLE2_H_
#define PARTICLE2_H_

// <<<<<<<<<< includes >>>>>>>>>> //
#include <glm/glm.hpp>

class Particle {

private:

	glm::vec3 gridIndex;//
	int partIndex;//

	glm::vec3 position;//
	glm::vec3 velocity;//
	float mass;//
	glm::vec3 force;//

	static int indexCount; //used to populate global particle array

public:

	Particle(glm::vec3 posIN, float massIN);
	~Particle();

	float* calculateForces();	//mehrere schritte zusammenfassen

	void updateVelocity(float* bodyPosition, float* bodyVelocity, float* bodyAngularVelocity);
	void applyRotation(glm::mat3 rotatMatrix, glm::vec3 relatPos, glm::vec3 bodyPos);
	void populateArray();

	void reset(float* oldBodyPos, float* newBodyPos);

	void updateGridIndex();

	// <<<<<<<<<< getter + setter >>>>>>>>>> //
};

#endif /* PARTICLE2_H_ */
