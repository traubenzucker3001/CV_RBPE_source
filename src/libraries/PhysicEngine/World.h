
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
 * class for the physics simulation world representation
 */
//class World : public Singleton<World> {
	//friend class Singleton<World>;

class World{

private:
	float worldSize;					/**< world size */
	float partRadius;					/**< particle radius */

	float springCoeff;					/**< spring coefficient */

	float dampCoeff;					/**< damping coefficient */		//welt oder jeder body einzeln?!
	float gravity;						/**< gravity force value */

	//RigidBody** allBodies;			/**< list of all bodies in the world */
	int allBodyNum;						/**< number of bodies in the world */
	
	//Particle** allParticles;			/**< list of all particles in the world */
	int allPartNum;						/**< number of particles in the world */

	UniformGrid *grid;					/**< uniform grid */

	//Cuda *cudaStruct;					/**< cuda data structure */
	
public:

	//private or public?!
	RigidBody **allBodies;				/**< list of all bodies in the world */
	Particle **allParticles;			/**< list of all particles in the world */

	/** \brief constructor
	 *
	 * creates the world instance
	 * @param wsIN world size
	 * @param prIN particle radius
	 * @param scIN spring coefficient
	 * @param dcIN damping coefficient
	 * @param bNum number of bodies
	 */
	World(float wsIN, float prIN, float scIN, float dcIN, int bNum);

	/** \brief default constructor
	*
	* creates the world instance
	* -not implemented-
	*/
	World();

	/** \brief destructor
	 *
	 * destroys the world instance
	 */
	~World();

	/** \brief step physics simulation
	 *
	 * ...
	 * @param duration time length of a simulation step
	 * @param isGPU true if simulation should run on gpu, false for simulation on cpu
	 * @return void
	 */
	void stepPhysics(float duration, bool isGPU);

	// <<<<<<<<<< getter + setter >>>>>>>>>> //
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

	void setAllBodies( RigidBody**& allBodies) {
		this->allBodies = allBodies;
	}

	 Particle**& getAllParticles() {
		return allParticles;
	}

	void setAllParticles( Particle**& allParticles) {
		this->allParticles = allParticles;
	}
};

#endif /* WORLD_H_ */
