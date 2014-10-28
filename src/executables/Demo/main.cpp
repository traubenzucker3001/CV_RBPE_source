#include <iostream>
#include "CVK_Framework\CVK_Framework.h"

#include <cuda.h>
#include <cuda_runtime.h>

#include "DemoApp\Demo2.h"

using namespace std;

#define WIDTH 800
#define HEIGHT 800

//simulations parameter
Demo *demo;
float dur = 0.01f;
float termVeloc = 20.0f;
float springCoeff = 100.0f;
float dampCoeff = 0.5f;		//welt oder jeder body einzeln?!
bool isGPU = false;

int main() {

	demo = new Demo(WIDTH, HEIGHT,dur,termVeloc);
	demo->run();

	//cout << "Hallo Test!" << endl;

	cudaDeviceReset();
	return 0;
}

//oder demo in einzelnes neues projekt??!
//nein, so lassen


//----- example1 -----

//sehen ob in main oder demo.cpp
/*
void resizeCallback(GLFWwindow *window, int w, int h)
{
	camera.setWidthHeight(w, h);
	glViewport(0, 0, w, h);
}
*/

/*
int main()
{
	//opengl parameter

	//Main node in hierarchy
	CVK::Node* earthNode = new CVK::Node("Earth");
	earthNode->setGeometry(teapot);
	earthNode->setMaterial(&mat_cvlogo);

	//Node in relation to earthNode
	CVK::Node* moonNode = new CVK::Node("Moon");
	moonNode->setGeometry(sphere);
	moonNode->setMaterial(&mat_brick);
	moonNode->setModelMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(3, 0, 0)));

	earthNode->addChild(moonNode);

	//set camera

}
*/
