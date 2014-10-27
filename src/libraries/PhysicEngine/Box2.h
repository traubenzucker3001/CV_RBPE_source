
#ifndef BOX2_H_
#define BOX2_H_

// <<<<<<<<<< includes >>>>>>>>>> //
#include "CollisionShape2.h"

/** \brief Box
 *
 * CollisionShape in box form
 */
class Box : public CollisionShape {

private:

	float halfsize;	/**< defines the size of the box */

public:

	/** \brief constructor
	 *
	 * ...
	 */
	Box(glm::vec3 origIN, float massIN, Particle* bPartIN, float numPartIN, float halfsizeIN) : CollisionShape(origIN,massIN,bPartIN,numPartIN){halfsize = halfsizeIN;}

	/** \brief destructor
	 *
	 * ...
	 */
	virtual ~Box();

	/** \brief create particles
	 *
	 * ...
	 */
	void calcParticles();

	/** \brief apply rotation to body particles
	 *
	 * ...
	 */
	void applyRotToPart(glm::mat3 rotMatrix);

	/** \brief fill particle array
	 *
	 * ...
	 */
	void populateParticleArray();	//virtual?!


	// <<<<<<<<<< getter + setter >>>>>>>>>> //
};

#endif /* BOX2_H_ */
