
// <<<<<<<<<< includes >>>>>>>>>> //
#include <iostream>

#include "Timing2.h"

Timing::Timing(){

	frameNum = 0;
	fps = 0;
	startTime = 0;
	endTime = 0;
	totalTime = 0;
	averageFrameDuration = 0;

}

Timing::~Timing(){

	//...
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
	currTime = glfwGetTime();

	return currTime;
}

void Timing::updateAFD(){

	//...
}

int Timing::updateFPS(){

	fps = frameNum / ((float)totalTime / 1000.0f);

	frameNum = 0;
	totalTime = 0;

	return fps;
}
