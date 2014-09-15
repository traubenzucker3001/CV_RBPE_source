#ifndef CORE_H
#define CORE_H

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "World.h"

/** \brief Engine Core
 *
 * initial class. every "important" configs can be done/set here.
 * Core is Singleton to ensure that there is only one engine core at a time
 */
class Core : public Singleton<World> {
	friend class Singleton<World>;

private:
	World physicsWorld;			/**< physics world that contains all rigid bodies */

	double velocityIterations;	/**< defines number of velocity iteration steps */

	double velocityEpsilon;		/**< ... */

	double positionIterations;	/**< defines number of position iteration steps */

	double positionEpsilon;		/**< ... */

	bool calcIterations;		/**< ... */

	unsigned int maxContacts;	/**< defines max number of contacts in the world */


public:

	/** \brief default constructor
	 *
	 * ...
	 */
	Core();

	/** \brief constructor
	 *
	 * ...
	 */
	Core(double viIN, double veIN , double piIN, double peIN, bool ciIN, unsigned int mcIN);

	/** \brief destructor
	 *
	 * ...
	 */
	~Core();

	/** \brief creates the Engine
	 *
	 * sets "important" values and initials every necessary part of the Engine
	 */
	void createEngine(double viIN, double veIN , double piIN, double peIN, bool ciIN, unsigned int mcIN);

};
#endif
