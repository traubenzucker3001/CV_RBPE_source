
#ifndef BOX2_H_
#define BOX2_H_

#include "CollisionShape2.h"

class Box : public CollisionShape {

private:

	float halfsize;	/**< defines the size of the box */

public:

	Box(glm::vec3 origIN, float halfsizeIN) : CollisionShape(origIN){halfsize = halfsizeIN;}
	virtual ~Box();

	void calcParticles();
	void applyRotatToPart(glm::mat3 rotMatrix);

	//void populateParticleArray();	//virtual?!

	// <<<<<<<<<< getter + setter >>>>>>>>>> //

};

#endif /* BOX2_H_ */
