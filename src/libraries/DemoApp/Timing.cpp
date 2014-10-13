#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "Timing.h"


Timing::Timing(){

	frameNum = NULL;
	lastFrameTimeStamp = NULL;
	lastFrameDuration = NULL;
	lastFrameClockStamp = NULL;
	lastFrameClockTicks = NULL;
	averageFrameDuration = NULL;
	framesPerSec = NULL;
}

Timing::~Timing(){

	/*
	delete frameNum;
	delete lastFrameTimeStamp;
	delete lastFrameDuration;
	delete lastFrameClockStamp;
	delete lastFrameClockTicks;
	delete averageFrameDuration;
	delete framesPerSec;
	*/
}

//wenn konstruktor habe, dann brauch eig kein init
void Timing::initTime(){

	frameNum = 0;

	//get system time
	//lastFrameTimeStamp = systemTime;

	lastFrameDuration = 0;

	//get system clock
	//lastFrameClockStamp = systemClock;

	lastFrameClockTicks = 0;
	averageFrameDuration = 0;
	framesPerSec = 0;
}

void Timing::updateTime(){
}

unsigned int Timing::getTime(){
	return 0;
}

unsigned long Timing::getClock(){
	return 0;
}
