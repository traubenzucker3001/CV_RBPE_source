
// <<<<<<<<<< includes >>>>>>>>>> //
#include <GL/glew.h>
#include <GLFW\glfw3.h>
#include <iostream>

#include "Timing.h"

using namespace std;

Timing::Timing(){

	frameNum = 0;
	fps = 0;
	startTime = 0;
	endTime = 0;
	totalTime = 0;
	afd = 0;

}

Timing::~Timing(){

	//no pointers to clear!
}

void Timing::startFrame(){

	startTime = getTime();
	frameNum++;
}

void Timing::endFrame(){

	endTime = getTime();
	totalTime = totalTime + (endTime - startTime);
}

int Timing::getTime(){

	int currTime;
	currTime = (int)glfwGetTime();

	return currTime;
}

void Timing::updateAFD(){

	//...
}

float Timing::updateFPS(){

	fps = frameNum / ((float)totalTime / 1000.0f);

	frameNum = 0;
	totalTime = 0;

	return fps;
}
