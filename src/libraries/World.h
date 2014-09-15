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

#include "designPatterns/Singleton.h"

/** \brief Physics World
 *
 * contains all rigid bodies.
 * PhysicWorld is Singleton to ensure that there is only one physic world at a time
 */
class World : public Singleton<World> {
friend class Singleton<World>;

private:
	Contact contacts;

	ForceRegistry forceReg;

	ContactGen contactGen;

	ContactResolver resolver;

	RigidBody bodies;


public:
	void genContacts();

	void runPhysics();

	void startFrame();

};
#endif
