#ifndef DEMO_H
#define DEMO_H

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "PhysicEngine/VirtualObject.h"
#include "PhysicEngine/Core.h"
#include "DemoApp/Timing.h"

/** \brief Demo
 *
 * ...
 */
class Demo
{
private:
	std::vector <VirtualObject*> virtualObjs;	/**< list of virtual objects */

	Core *physicsCore;							/**< physics engine core */

	int windowWidth;							/**< window width */

	int windowHeight;							/**< window height */

	Timing *time;								/**< time data */


public:
	/** \brief initial physics
	 *
	 * ...
	 */
	void initPhysics();

	/** \brief initial graphics
	 *
	 * ...
	 */
	void initGraphics();

	/** \brief display function
	 *
	 * called each frame, ...
	 */
	void display();

	/** \brief update
	 *
	 * called each frame, ...
	 */
	void update();

	/** \brief key callback function
	 *
	 * ...
	 */
	void keyCallback();

	/** \brief window resize callback function
	 *
	 * ...
	 */
	void resizeCallback();

	/** \brief mouse button callback function
	 *
	 * ...
	 */
	void mouseButtonCallback();

	/** \brief mouse motion callback function
	 *
	 * ...
	 */
	void mouseMotionCallback();

};
#endif
