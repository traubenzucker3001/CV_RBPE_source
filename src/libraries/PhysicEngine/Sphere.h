
#ifndef SPHERE_H_
#define SPHERE_H_

// <<<<<<<<<< includes >>>>>>>>>> //
#include "CollisionShape.h"

/** \brief Sphere
 *
 * CollisionShape in sphere form
 * -currently not used-
 */
class Sphere : public CollisionShape {

private:

	float radius;	/**< defines the size of the sphere */

public:

	/** \brief constructor
	 *
	 * creates a collision shape as a sphere instance
	 * @param origIN position
	 * @param massIN mass
	 * @param bPartIN particle array
	 * @param numPartIN number of particles inside the array
	 * @param radiusIN size of the sphere
	 */
	Sphere(glm::vec3 origIN, float massIN, Particle** bPartIN, int numPartIN, float radiusIN) : CollisionShape(origIN,massIN,bPartIN,numPartIN){radius = radiusIN;}

	/** \brief destructor
	 *
	 * destroys a box instance
	 */
	virtual ~Sphere();

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

#endif /* SPHERE_H_ */
