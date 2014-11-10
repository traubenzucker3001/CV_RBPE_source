
#ifndef WORLD_H_
#define WORLD_H_

// <<<<<<<<<< includes >>>>>>>>>> //
#include <vector>

#include "RigidBody.h"
#include "UniformGrid.h"
#include "DesignPatterns\Singleton.h"
#include "Cuda.h"
#include <GL\glew.h>

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

	//std::vector<RigidBody*> allBodies;	//array wahrscheinl. besser
	RigidBody** allBodies;				/**< list of all bodies in the world */
	int allBodyNum;						/**< number of bodies in the world */
	//std::vector<Particle*> allParticles;	//array wahrscheinl. besser
	//**
	Particle** allParticles;			/**< list of all particles in the world */
	int allPartNum;						/**< number of particles in the world */

	UniformGrid *grid;					/**< uniform grid */

	Cuda *cudaStruct;					/**< cuda data structure */

	//in demo gepackt
	//GLuint rbVBO;						/**< vertex buffer object for rigid body positions */
	//GLuint pVBO;						/**< vertex buffer object for particle positions */
	//?
	//int vertexCount;					/**< ... */
	
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
	void stepPhysics(float duration, bool isGPU);

	// <<<<<<<<<< getter + setter >>>>>>>>>> //
	 Particle** getAllParticles() const {
		return allParticles;
	}

	void setAllParticles( Particle**& allParticles) {
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

	int getAllBodyNum() const {
		return allBodyNum;
	}

	void setAllBodyNum(int allbodyNum) {
		this->allBodyNum = allbodyNum;
	}

	RigidBody**& getAllBodies() {
		return allBodies;
	}

	void setAllBodies(RigidBody**& allBodies) {
		this->allBodies = allBodies;
	}
};

#endif /* WORLD_H_ */
