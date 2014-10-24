/*
 * Particle2.h
 *
 *  Created on: 24.10.2014
 *      Author: Daniel
 */

#ifndef PARTICLE2_H_
#define PARTICLE2_H_

#include <glm/glm.hpp>

class Particle {

private:

	glm::vec3 gridIndex;//
	int particleIndex;//

	glm::vec3 position;//
	glm::vec3 velocity;//
	float mass;//
	glm::vec3 force;//

	static int indexCount; //used to populate global particle array

public:

	Particle();
	~Particle;

	//TODO funcs

};

#endif /* PARTICLE2_H_ */
