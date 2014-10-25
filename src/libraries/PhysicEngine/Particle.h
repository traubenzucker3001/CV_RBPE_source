/*
#ifndef PARTICLE_H_
#define PARTICLE_H_

#include <glm/glm.hpp>

/** \brief Partikel
 *
 * ...
 */
/*class Particle {

private:
	int gridIndex[3];	/**< ... */

/*	int partIndex;

	glm::vec3 position;

	glm::vec3  velocity;

	float mass;

	glm::vec3 force;

	static int indexCount; //used to populate global particle array

	void calculateCollisionForces();
	void calculateCollisionForcesWithGrid();
	void calculateBoundaryForces();
public:
	/** \brief Demo
	 *
	 * ...
	 */
/*	Particle();
	Particle(glm::vec3 pos, float mass);

	void populateArray();
	void updateVelocity(glm::vec3 bodyPosition, glm::vec3 bodyVelocity, glm::vec3 bodyAngularVelocity);
	void applyRotation(glm::mat3 rotationMatrix, glm::vec3 originalRelativePos, glm::vec3 bodyPosition);
	float* calculateForces();
	void reset(float* oldBodyPos, float* newBodyPos);

	void updateGridIndex();

	// <<<<<<<<<< getter + setter >>>>>>>>>> //
	const int* getGridIndex() const {
		return gridIndex;
	}
};

#endif /* PARTICLE_H_ */
