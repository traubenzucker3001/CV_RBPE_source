
#ifndef WORLD2_H_
#define WORLD2_H_

// <<<<<<<<<< includes >>>>>>>>>> //
#include "UniformGrid2.h"

class World : public Singleton<World> {
	friend class Singleton<World>;

private:
	float worldSize;
	float partRadius;

	std::vector<RigidBody*> allBodies;
	int allbodyNum;
	//std::vector<Particle*> allParticles;	//array wahrscheinl. besser
	Particle* allParticles;	//**
	int allPartNum;

	UniformGrid *grid;

public:

	World(float wsIN, float prIN);
	~World();

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
};

#endif /* WORLD2_H_ */
