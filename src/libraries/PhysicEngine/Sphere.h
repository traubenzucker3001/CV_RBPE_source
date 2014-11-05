
#ifndef SPHERE_H_
#define SPHERE_H_

// <<<<<<<<<< includes >>>>>>>>>> //
#include "CollisionShape.h"

/** \brief Sphere
 *
 * CollisionShape in sphere form
 */
class Sphere : public CollisionShape {

private:

	float radius;	/**< defines the size of the sphere */

public:

	/** \brief constructor
	 *
	 * ...
	 */
	Sphere(glm::vec3 origIN, float massIN, Particle* bPartIN, float numPartIN, float radiusIN) : CollisionShape(origIN,massIN,bPartIN,numPartIN){radius = radiusIN;}

	/** \brief destructor
	 *
	 * ...
	 */
	virtual ~Sphere();

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
	void populatePartArray();	//virtual?!

	// <<<<<<<<<< getter + setter >>>>>>>>>> //
};

#endif /* SPHERE_H_ */
