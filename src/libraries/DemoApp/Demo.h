
//TODO callback funcs

#ifndef DEMO_H_
#define DEMO_H_

// <<<<<<<<<< includes >>>>>>>>>> //
#include <iostream>
#include <vector>

#include "Timing.h"
#include "PhysicEngine\World.h"
#include "PhysicEngine\VirtualObject.h"
#include "DesignPatterns\Singleton.h"

#include "CVK_Framework\CVK_Framework.h"

// <<<<<<<<<< callback funcs >>>>>>>>>> //
/** \brief window resize callback
*
* called whenever the window size is changed
* @param window the glfw window reference
* @param w new windoe width
* @param h new window height
* @return void
*/
void resizeCallback(GLFWwindow *window, int w, int h);

/** \brief Demo
 *
 * frame for a demo application, contains all important init-/simulation-/callback- functions and parameters.
 */
//class Demo : public Singleton<Demo> {
	//friend class Singleton<Demo>;

class Demo{

private:
	int windowWidth;							/**< demo window width */
	int windowHeight;							/**< demo window height */

	//World *physicsWorld;						/**< physics world that contains all rigid bodies */

	//oder auch array?!
	std::vector <VirtualObject*> virtualObjs;	/**< list of virtual objects */
	int virtObjNum;								/**< number of virtual objects in the list */

	Timing *time;								/**< time data */
	float duration;								/**< step duration */
	float terminalVeloc;						/**< terminal velocity */

	bool isGPU;									/**< true if simulation should run on GPU, falso if simulation should run on CPU */
	bool withGrid;								/**< ... */
	bool renderPart;							/**< ... */

	//?
	/*int vertexCount;					/**< ... */

public:

	CVK::Trackball *camera;				/**< cvk camera objekt */	//bzw normale camera

	CVK::Node* sceneRoot;				/**< ... */
	CVK::Geometry* cubeGeometry;		/**< ... */
	CVK::Plane* plane;					/**< ... */
	CVK::Material* cubeMaterial;		/**< ... */
	CVK::Node* partRoot;				/**< ... */
	CVK::Geometry* partGeometry;		/**< ... */
	CVK::Material* partMaterial;		/**< ... */

	/** \brief constructor
	*
	* creates the demo instance
	* @param wwIN window width
	* @param whIN window height
	* @param durIN duration
	* @param tvIN terminal velocity
	* @param wsIN world size
	* @param prIN particle radius
	* @param scIN spring coefficient
	* @param dcIN damping coefficient
	* @param bnIN total number of bodies in the scene
	* @param igIN true for executuin on gpu, false for cpu
	*/
	Demo(int wwIN, int whIN, float durIN, float tvIN, float wsIN, float prIN, float scIN, float dcIN, int bnIN, bool igIN, bool wgIN, bool rpIN);

	/** \brief default constructor
	*
	* creates the demo instance
	* -not implemented-
	*/
	Demo();

	/** \brief destructor
	*
	* destroys the demo instance
	*/
	~Demo();


	/** \brief init physics
	*
	* ...
	*/
	//void initPhysics();	//eig schon im konstruktor von demo gemacht

	/** \brief init graphics
	*
	* run the demo initialization and main loop
	* @return void
	*/
	void run();	//f¸r cvk stuff		//vorher initGraphics()

	/** \brief init scene
	*
	* create uniform grid when running on cpu and initialize scene with objects
	* @return void
	*/
	void initScene();

	/** \brief update simulation
	*
	* step the physics simulation main loop
	* @param duration time length of one simulation step
	* @return void
	*/
	void stepSimulation(float duration);

	/** \brief reset scene
	*
	* reset the scene and their objects
	* -not yet implemented-
	* @return void
	*/
	void resetScene();

	/** \brief display function
	*
	* ...
	*/
	//void display();	//nicht nˆtig

	/** \brief update virtual objects
	*
	* update modelmatrix from all virtual objects
	*/
	void updateVOs();


	// <<<<<<<<<< callback funcs >>>>>>>>>> //
	//schauen wie das mit cvk l‰uft
	/** \brief mouse button callback
	*
	* calls whenever a mouse button is pressed
	* @param button mouse button
	* @param state mouse button state (pressed, etc.)
	* @param x mouse x coordinates
	* @param y mouse y coordinates
	* @return void
	*/
	void mouseButtonCallback(int button, int state, int x, int y);

	/** \brief mouse motion callback
	*
	* calls whenever the mouse is moved
	* @param x mouse x coordinates
	* @param y mouse y coordinates
	* @return void
	*/
	void mouseMotionCallback(int x, int y);

	/** \brief key callback
	*
	* calls whenever a keyboard button is pressed
	* @param key pressed keyboard button
	* @param x mouse x coordinates
	* @param y mouse y coordinates
	* @return void
	*/
	void keyCallback(unsigned char key, int x, int y);

	//void resizeCallback(GLFWwindow *window, int w, int h);		//auﬂerhalb klasse gelegt!


	// <<<<<<<<<< getter + setter >>>>>>>>>> //
	float getTerminalVeloc() const {
		return terminalVeloc;
	}

	void setTerminalVeloc(float terminalVeloc) {
		this->terminalVeloc = terminalVeloc;
	}

	float getDuration() const {
		return duration;
	}

	void setDuration(float duration) {
		this->duration = duration;
	}

	bool isIsGpu() const {
		return isGPU;
	}

	void setIsGpu(bool isGpu) {
		isGPU = isGpu;
	}

	bool isWithGrid() const {
		return withGrid;
	}

	void setWithGrid(bool withGrid) {
		this->withGrid = withGrid;
	}
};

#endif /* DEMO_H_ */
