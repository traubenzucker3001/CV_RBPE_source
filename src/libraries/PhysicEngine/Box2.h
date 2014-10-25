
#ifndef BOX2_H_
#define BOX2_H_

// <<<<<<<<<< includes >>>>>>>>>> //
#include "CollisionShape2.h"

class Box : public CollisionShape {

private:

	float halfsize;	/**< defines the size of the box */

public:

	Box(glm::vec3 origIN, float massIN, Particle* bPartIN, float numPartIN, float halfsizeIN) : CollisionShape(origIN,massIN,bPartIN,numPartIN){halfsize = halfsizeIN;}
	virtual ~Box();

	void calcParticles();
	void applyRotatToPart(glm::mat3 rotMatrix);

	void populateParticleArray();	//virtual?!

	// <<<<<<<<<< getter + setter >>>>>>>>>> //

};

#endif /* BOX2_H_ */
