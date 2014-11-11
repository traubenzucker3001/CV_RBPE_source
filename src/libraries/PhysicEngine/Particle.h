
#ifndef PARTICLE_H_
#define PARTICLE_H_

// <<<<<<<<<< includes >>>>>>>>>> //
#include <glm/glm.hpp>

/** \brief Particle
 *
 * class for the particle representation
 */
class Particle {

private:

	glm::vec3 gridIndex;	/**< particle grid index */
	int partIndex;			/**< particle index */

	glm::vec3 position;		/**< particle position */
	glm::vec3 velocity;		/**< particle velocity */
	float mass;				/**< particle mass */
	glm::vec3 force;		/**< particle force */

	static int indexCount; 	/**< help variable to populate all particle array */	//used to populate global particle array

public:

	/** \brief constructor
	 *
	 * creates a particle instance
	 * @param posIN particle position
	 * @param massIN particle mass
	 */
	Particle(glm::vec3 posIN, float massIN);

	/** \brief destructor
	 *
	 * destroys a particle instance
	 */
	~Particle();

	/** \brief calculate forces
	 *
	 * calculate forces with colliding neighbor particles and grid boundries
	 * @return current calculated force
	 */
	glm::vec3 calculateForces();	//mehrere schritte zusammenfassen

	/** \brief update particle velocity
	 *
	 * update particle velocity with the new rigid body values
	 * @param bodyPosition corresponding rigid body position
	 * @param bodyVelocity corresponding rigid body velocity
	 * @param bodyAngularVelocity corresponding rigid body angular velocity
	 * @return void
	 */
	void updateVeloc(glm::vec3 bodyPosition, glm::vec3 bodyVelocity, glm::vec3 bodyAngularVelocity);

	/** \brief apply rotation
	 *
	 * apply rotation to particle relative to the shapes origin
	 * @param rotatMatrix rotation matrix
	 * @param relatPos particles relative position to the shapes origin
	 * @param bodyPos shape origin
	 * @return void
	 */
	void applyRot(glm::mat3 rotatMatrix, glm::vec3 relatPos, glm::vec3 bodyPos);

	/** \brief fill array
	 *
	 * populate the world particle array
	 * @return void
	 */
	void populateArray();

	/** \brief reset particle
	 *
	 * reset a particle of an objekt
	 * -not yet implemented-
	 * @param oldBodyPos
	 * @param newBodyPos
	 * @return void
	 */
	void reset(float* oldBodyPos, float* newBodyPos);

	/** \brief update grid index
	 *
	 * update uniform grid indices
	 * @return void
	 */
	void updateGridIndex();


	// <<<<<<<<<< gpu parts >>>>>>>>>> //
	/** \brief update cuda array
	 *
	 * ...
	 * @param particleIndex particle index in uniform grid
	 * @return void
	 */
	void updateCUDArray(int particleIndex);


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

#endif /* PARTICLE_H_ */
