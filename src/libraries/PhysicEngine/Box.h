
#ifndef BOX_H_
#define BOX_H_

// <<<<<<<<<< includes >>>>>>>>>> //
#include "CollisionShape.h"

/** \brief Box
 *
 * CollisionShape in box form
 */
class Box : public CollisionShape {

private:

	float halfsize;		/**< defines the size of the box */

public:

	/** \brief constructor
	 *
	 * creates a collision shape as a box instance
	 * @param origIN position
	 * @param massIN mass
	 * @param bPartIN particle array
	 * @param numPartIN number of particles inside the array
	 * @param halfsizeIN size of the box
	 */
	Box(glm::vec3 origIN, float massIN, Particle** bPartIN, int numPartIN, float halfsizeIN) : CollisionShape(origIN,massIN,bPartIN,numPartIN){halfsize = halfsizeIN;}

	/** \brief destructor
	 *
	 * destroys a box instance
	 */
	virtual ~Box();

	/** \brief create particles
	 *
	 * create body particles and calculate their position relative to shape origin and their mass
	 * @return void
	 */
	void calcParticles();

	/** \brief apply rotation to body particles
	 *
	 * apply rotation to particles relative to the shape origin
	 * @param rotMatrix rotation matrix
	 * @return void
	 */
	void applyRotToPart(glm::mat3 rotMatrix);

	/** \brief fill particle array
	 *
	 * fill the world particle array
	 * @return void
	 */
	void populatePartArray();	//virtual?!


	// <<<<<<<<<< getter + setter >>>>>>>>>> //
};

#endif /* BOX_H_ */
