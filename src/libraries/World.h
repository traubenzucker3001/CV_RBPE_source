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
 * World is Singleton to ensure that there is only one physic world at a time
 */
class World : public Singleton<World> {
	friend class Singleton<World>;

private:
	Contact contacts;			/**< list of all contacts inside the world */

	ForceRegistry forceReg;		/**< registry that contains information about  which forces appear to which rigid bodies */

	ContactGen contactGen;		/**< class which updates the contact list in each iteration */

	ContactResolver resolver;	/**< contact resolution */

	RigidBody bodies;			/**< list of all rigid bodies in the world */


public:
	/** \brief
	 *
	 * ...
	 */
	void genContacts();

	/** \brief
	 *
	 * ...
	 */
	void runPhysics();

	/** \brief
	 *
	 * ...
	 */
	void startFrame();

};
#endif
