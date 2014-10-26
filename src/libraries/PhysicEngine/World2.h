
#ifndef WORLD2_H_
#define WORLD2_H_

// <<<<<<<<<< includes >>>>>>>>>> //
#include <vector>

#include "RigidBody2.h"
#include "UniformGrid2.h"
#include "DesignPatterns\Singleton.h"

/** \brief physics World
 *
 * ...
 */
class World : public Singleton<World> {
	friend class Singleton<World>;

private:
	float worldSize;					/**< world size */
	float partRadius;					/**< particle radius */

	float springCoeff;					/**< spring coefficient */
	//welt oder jeder body einzeln?!
	float dampCoeff;					/**< damping coefficient */
	float gravity;						/**< gravity force value */

	std::vector<RigidBody*> allBodies;	/**< list of all bodies in the world */
	int allbodyNum;						/**< number of bodies in the world */
	//std::vector<Particle*> allParticles;	//array wahrscheinl. besser
	//**
	Particle** allParticles;				/**< list of all particles in the world */
	int allPartNum;						/**< number of particles in the world */

	UniformGrid *grid;					/**< uniform grid */

public:

	/** \brief constructor
	 *
	 * ...
	 */
	World(float wsIN, float prIN, float scIN, float dcIN);

	/** \brief destructor
	 *
	 * ...
	 */
	~World();

	/** \brief step physics simulation
	 *
	 * ...
	 */
	void stepPhysics(float duration);

	// <<<<<<<<<< getter + setter >>>>>>>>>> //
	const Particle*& getAllParticles() const {
		return allParticles;
	}

	void setAllParticles(const Particle*& allParticles) {
		this->allParticles = allParticles;
	}

	int getAllPartNum() const {
		return allPartNum;
	}

	void setAllPartNum(int allPartNum) {
		this->allPartNum = allPartNum;
	}

	float getPartRadius() const {
		return partRadius;
	}

	void setPartRadius(float partRadius) {
		this->partRadius = partRadius;
	}

	float getWorldSize() const {
		return worldSize;
	}

	void setWorldSize(float worldSize) {
		this->worldSize = worldSize;
	}

	float getSpringCoeff() const {
		return springCoeff;
	}

	void setSpringCoeff(float springCoeff) {
		this->springCoeff = springCoeff;
	}

	float getDampCoeff() const {
		return dampCoeff;
	}

	void setDampCoeff(float dampCoeff) {
		this->dampCoeff = dampCoeff;
	}

	float getGravity() const {
		return gravity;
	}

	void setGravity(float gravity) {
		this->gravity = gravity;
	}
};

#endif /* WORLD2_H_ */
