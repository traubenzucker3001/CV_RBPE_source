
// <<<<<<<<<< includes >>>>>>>>>> //
#include <iostream>
#include <GLFW\glfw3.h>

#include "Timing.h"

using namespace std;

Timing::Timing(){

	cout << "time: timing constr called!" << endl; //zum test

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

	cout << "time: start frame!" << endl; //zum test

	startTime = getTime();
	frameNum++;
}

void Timing::endFrame(){

	cout << "time: end frame!" << endl; //zum test

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
