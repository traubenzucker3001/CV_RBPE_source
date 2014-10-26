
#ifndef PARTICLE2_H_
#define PARTICLE2_H_

// <<<<<<<<<< includes >>>>>>>>>> //
#include <glm/glm.hpp>

/** \brief Particle
 *
 * ...
 */
class Particle {

private:

	glm::vec3 gridIndex;//	/**< particle grid index */
	int partIndex;//		/**< particle index */

	glm::vec3 position;//	/**< particle position */
	glm::vec3 velocity;//	/**< particle velocity */
	float mass;//			/**< particle mass */
	glm::vec3 force;//		/**< particle force */

	//used to populate global particle array
	static int indexCount; 	/**< help variable to populate all particle array */

public:

	/** \brief constructor
	 *
	 * ...
	 */
	Particle(glm::vec3 posIN, float massIN);

	/** \brief destructor
	 *
	 * ...
	 */
	~Particle();

	/** \brief calculate forces
	 *
	 * calculate forces with colliding neighbor particles and grid boundries
	 */
	glm::vec3* calculateForces();	//mehrere schritte zusammenfassen

	/** \brief update particle velocity
	 *
	 * ...
	 */
	void updateVeloc(glm::vec3 bodyPosition, glm::vec3 bodyVelocity, glm::vec3 bodyAngularVelocity);

	/** \brief apply rotation
	 *
	 * ...
	 */
	void applyRot(glm::mat3 rotatMatrix, glm::vec3 relatPos, glm::vec3 bodyPos);

	/** \brief fill array
	 *
	 * ...
	 */
	void populateArray();

	/** \brief reset particle
	 *
	 * ...
	 */
	void reset(float* oldBodyPos, float* newBodyPos);

	/** \brief update grid index
	 *
	 * ...
	 */
	void updateGridIndex();

	// <<<<<<<<<< getter + setter >>>>>>>>>> //
	const glm::vec3& getPosition() const {
		return position;
	}

	void setPosition(const glm::vec3& position) {
		this->position = position;
	}

	const glm::vec3& getVelocity() const {
		return velocity;
	}

	void setVelocity(const glm::vec3& velocity) {
		this->velocity = velocity;
	}

	const glm::vec3& getGridIndex() const {
		return gridIndex;
	}

	void setGridIndex(const glm::vec3& gridIndex) {
		this->gridIndex = gridIndex;
	}
};

#endif /* PARTICLE2_H_ */
