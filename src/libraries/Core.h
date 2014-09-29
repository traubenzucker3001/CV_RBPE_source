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
class Core : public Singleton<Core> {
	friend class Singleton<Core>;

private:
	World *physicsWorld;		/**< physics world that contains all rigid bodies */

	double velocityIterations;	/**< defines number of velocity iteration steps */

	double velocityEpsilon;		/**< ... */

	double positionIterations;	/**< defines number of position iteration steps */

	double positionEpsilon;		/**< ... */

	bool calcIterations;		/**< ... */

	unsigned int maxContacts;	/**< defines max number of contacts in the world */


public:

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

	/** \brief change engine parameters
	 *
	 * sets "important" values and initials every necessary part of the Engine
	 */
	/*
	void changeEngine(double viIN, double veIN , double piIN, double peIN, bool ciIN, unsigned int mcIN);
	*/

	// <<<<<<<<<< getter + setter >>>>>>>>>> //
	bool isCalcIterations() const {
		return calcIterations;
	}

	void setCalcIterations(bool calcIterations) {
		this->calcIterations = calcIterations;
	}

	unsigned int getMaxContacts() const {
		return maxContacts;
	}

	void setMaxContacts(unsigned int maxContacts) {
		this->maxContacts = maxContacts;
	}

	double getPositionEpsilon() const {
		return positionEpsilon;
	}

	void setPositionEpsilon(double positionEpsilon) {
		this->positionEpsilon = positionEpsilon;
	}

	double getPositionIterations() const {
		return positionIterations;
	}

	void setPositionIterations(double positionIterations) {
		this->positionIterations = positionIterations;
	}

	double getVelocityEpsilon() const {
		return velocityEpsilon;
	}

	void setVelocityEpsilon(double velocityEpsilon) {
		this->velocityEpsilon = velocityEpsilon;
	}

	double getVelocityIterations() const {
		return velocityIterations;
	}

	void setVelocityIterations(double velocityIterations) {
		this->velocityIterations = velocityIterations;
	}

};
#endif
