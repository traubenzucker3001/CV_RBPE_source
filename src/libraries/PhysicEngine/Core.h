#ifndef CORE_H
#define CORE_H

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "DesignPatterns/Singleton.h"
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

	float velocityIterations;	/**< defines number of velocity iteration steps */

	float velocityEpsilon;		/**< ... */

	float positionIterations;	/**< defines number of position iteration steps */

	float positionEpsilon;		/**< ... */

	bool calcIterations;		/**< ... */

	unsigned int maxContacts;	/**< defines max number of contacts in the world */

	int deviceID;				/**< ... */


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
	Core(float viIN, float veIN , float piIN, float peIN, bool ciIN, unsigned int mcIN);

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

	/** \brief 
	*
	* ...
	*/
	void initCUDA();

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

	float getPositionEpsilon() const {
		return positionEpsilon;
	}

	void setPositionEpsilon(float positionEpsilon) {
		this->positionEpsilon = positionEpsilon;
	}

	float getPositionIterations() const {
		return positionIterations;
	}

	void setPositionIterations(float positionIterations) {
		this->positionIterations = positionIterations;
	}

	float getVelocityEpsilon() const {
		return velocityEpsilon;
	}

	void setVelocityEpsilon(float velocityEpsilon) {
		this->velocityEpsilon = velocityEpsilon;
	}

	float getVelocityIterations() const {
		return velocityIterations;
	}

	void setVelocityIterations(float velocityIterations) {
		this->velocityIterations = velocityIterations;
	}

	int getDeviceId() const {
		return deviceID;
	}

	void setDeviceId(int deviceId) {
		deviceID = deviceId;
	}
};
#endif
