#ifndef DEMO_H
#define DEMO_H

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include <glm\glm.hpp>

#include "PhysicEngine/VirtualObject.h"
#include "PhysicEngine/Core.h"
#include "DemoApp/Timing.h"
#include "CVK_Framework/CVK_Framework.h"

/** \brief Demo
 *
 * ...
 */
class Demo : public Singleton<Demo> {
	friend class Singleton<Demo>;

private:
	std::vector <VirtualObject*> virtualObjs;	/**< list of virtual objects */

	Core *physicsCore;							/**< physics engine core */

	int windowWidth;							/**< window width */

	int windowHeight;							/**< window height */

	Timing *time;								/**< time data */

	GLFWwindow* window;

	CVK::Trackball camera;

	//nicht nur VO positionen, sondern auch größe der scene/simulationsdomain, um gitter richtig drüber legen zu können
	glm::vec3 sceneMIN;
	glm::vec3 sceneMAX;


public:

	/** \brief constructor
	*
	* ...
	*/
	Demo();

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

	//setView ?!

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

	// <<<<<<<<<< callback funcs >>>>>>>>>> //
	/** \brief key callback function
	 *
	 * ...
	 */
	void keyCallback();

	/** \brief window resize callback function
	 *
	 * ...
	 */
	void resizeCallback(GLFWwindow *window, int w, int h);

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
