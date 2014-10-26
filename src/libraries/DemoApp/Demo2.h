
//!noch cvk fehler!
//TODO callback funcs

#ifndef DEMO2_H_
#define DEMO2_H_

// <<<<<<<<<< includes >>>>>>>>>> //
#include <iostream>
#include <vector>

#include "Timing2.h"
#include "PhysicEngine/World2.h"
#include "PhysicEngine/VirtualObject.h"
#include "DesignPatterns/Singleton.h"

#include "CVK_Framework/CVK_Framework.h"

/** \brief Demo
 *
 * frame for a demo application, contains all important init-/simulation-/callback- functions and parameters.
 */
class Demo : public Singleton<Demo> {
	friend class Singleton<Demo>;

private:
	int windowWidth;							/**< demo window width */
	int windowHeight;							/**< demo window height */

	//nicht sicher ob als extra var nötig
	//GLFWwindow* window;						/**< glfw window reference */

	//oder noch über core gehen?!
	World *physicsWorld;						/**< physics world that contains all rigid bodies */

	std::vector <VirtualObject*> virtualObjs;	/**< list of virtual objects */
	int virtObjNum;								/**< number of virtual objects in the list */

	Timing *time;								/**< time data */
	float duration;								/**< step duration */
	float terminalVeloc;						/**< terminal velocity */

	//bzw normale camera
	CVK::Trackball camera;						/**< cvk camera objekt */


public:

	/** \brief constructor
	*
	* ...
	*/
	Demo(int wwIN, int whIN, float durIN, float tvIN);

	/** \brief destructor
	*
	* ...
	*/
	~Demo();


	/** \brief init physics
	*
	* ...
	*/
	//void initPhysics();	//eig schon im konstruktor von demo gemacht

	/** \brief init graphics
	*
	* ...
	*/
	void run();	//für cvk stuff		//vorher initGraphics()

	/** \brief init scene
	*
	* ...
	*/
	void initScene();

	/** \brief update simulation
	*
	* ...
	*/
	void stepSimulation(float duration);

	/** \brief reset scene
	*
	* ...
	*/
	void resetScene();

	/** \brief display function
	*
	* ...
	*/
	//void display();	//nicht nötig


	// <<<<<<<<<< callback funcs >>>>>>>>>> //
	/** \brief mouse button callback
	*
	* ...
	*/
	void mouseButtonCallback(int button, int state, int x, int y);

	/** \brief mouse motion callback
	*
	* ...
	*/
	void mouseMotionCallback(int x, int y);

	/** \brief key callback
	*
	* ...
	*/
	void keyCallback(unsigned char key, int x, int y);

	/** \brief window resize callback
	*
	* ...
	*/
	void resizeCallback(GLFWwindow *window, int w, int h);


	// <<<<<<<<<< getter + setter >>>>>>>>>> //
	float getTerminalVeloc() const {
		return terminalVeloc;
	}

	void setTerminalVeloc(float terminalVeloc) {
		this->terminalVeloc = terminalVeloc;
	}
};

#endif /* DEMO2_H_ */
