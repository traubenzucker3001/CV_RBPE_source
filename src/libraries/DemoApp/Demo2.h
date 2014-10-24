
#ifndef DEMO2_H_
#define DEMO2_H_

#include "CVK_Framework/CVK_Framework.h"

class Demo : public Singleton<Demo> {
	friend class Singleton<Demo>;

private:
	int windowWidth;
	int windowHeight;

	Timing *time;

	World *physicsWorld;	//oder noch über core gehen?!

	std::vector <VirtualObject*> virtualObjs;	/**< list of virtual objects */
	int virtObjNum;

	GLFWwindow* window;
	CVK::Trackball camera;	//bzw normale camera


public:

	Demo();
	~Demo();

	void initPhysics();
	//void initGraphics();	//für cvk stuff

	void initScene();
	void stepSimulation(float duration);
	void resetScene();

	void display();

	// <<<<<<<<<< callback funcs >>>>>>>>>> //
	void mouseButtonCallback(int button, int state, int x, int y);
	void mouseMotionCallback(int x, int y);
	void keyCallback(unsigned char key, int x, int y);
	void resizeCallback(GLFWwindow *window, int w, int h);
};

#endif /* DEMO2_H_ */
