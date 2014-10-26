//neue Klasse - World2.h

/*
#ifndef WORLD_H
#define WORLD_H

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "Contact.h"
#include "ForceRegistry.h"
#include "ContactGen.h"
#include "ContactResolver.h"
#include "RigidBody.h"
#include "Particle.h"

#include "designPatterns/Singleton.h"

/** \brief Physics World
 *
 * contains all rigid bodies.
 * World is Singleton to ensure that there is only one physic world at a time
 */
/*class World : public Singleton<World> {
	friend class Singleton<World>;

private:
	Contact *contacts;				/**< list of all contacts inside the world */

/*	ForceRegistry *forceReg;			/**< registry that contains information about  which forces appear to which rigid bodies */

/*	ContactGen *contactGen;				/**< class which updates the contact list in each iteration */

/*	ContactResolver *resolver;			/**< contact resolution */

/*	std::vector<RigidBody*> bodies;		/**< list of all rigid bodies in the world */


/*	int bodyNum;

	std::vector<Particle*> allParticles;

	int aPartNum;


public:
	/** \brief constructor
	 *
	 * ...
	 */
/*	World();

	/** \brief destructor
	 *
	 * ...
	 */
/*	~World();

	/** \brief
	 *
	 * ...
	 */
/*	unsigned int genContacts();

	/** \brief
	 *
	 * ...
	 */
/*	void runPhysics(float duration);

	/** \brief
	 *
	 * Initials world for a simulation frame. clears the force and torque accumulators for bodies.
	 */
/*	void startFrame();

	/** \brief
	 *
	 * adds new rigid body to the world
	 */
/*	void addBody(RigidBody *body);

	// <<<<<<<<<< getter + setter >>>>>>>>>> //
	const std::vector<Particle*>& getAllParticles() const {
		return allParticles;
	}

	void setAllParticles(const std::vector<Particle*>& allParticles) {
		this->allParticles = allParticles;
	}

	const std::vector<RigidBody*>& getBodies() const {
		return bodies;
	}

	void setBodies(const std::vector<RigidBody*>& bodies) {
		this->bodies = bodies;
	}

	int getBodyNum() const {
		return bodyNum;
	}

	void setBodyNum(int bodyNum) {
		this->bodyNum = bodyNum;
	}

	int getAPartNum() const {
		return aPartNum;
	}

	void setAPartNum(int partNum) {
		this->aPartNum = partNum;
	}
};
#endif
