
#ifndef WORLD2_H_
#define WORLD2_H_

class World : public Singleton<World> {
	friend class Singleton<World>;

private:
	float worldSize;

	std::vector<RigidBody*> allbodies;
	int allbodyNum;
	std::vector<Particle*> allParticles;
	int allPartNum;

public:

	World();
	~World();

	void stepPhysics(float duration);
};

#endif /* WORLD2_H_ */
