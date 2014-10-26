/*
#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include <time.h>

#include "Timing.h"


Timing::Timing(){

	frameNum = 0;
	lastFrameTimeStamp = 0;
	lastFrameDuration = 0;
	lastFrameClockStamp = 0;
	lastFrameClockTicks = 0;
	averageFrameDuration = 0;
	framesPerSec = 0;
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
/*}

//wenn konstruktor habe, dann brauch eig kein init
void Timing::initTime(){

	// Set up the timing system.

	// Create the frame info object

	frameNum = 0;

	//get system time
	//lastFrameTimeStamp = systemTime;
	lastFrameTimeStamp = getTime();

	lastFrameDuration = 0;

	//get system clock
	//lastFrameClockStamp = systemClock;
	lastFrameClockStamp = getClock();

	lastFrameClockTicks = 0;
	averageFrameDuration = 0;
	framesPerSec = 0;
}

void Timing::updateTime(){

	//frame number erhöhen
	frameNum++;

	//update time infos
	unsigned int timeTemp = getTime();
	lastFrameDuration = timeTemp - lastFrameTimeStamp;
	lastFrameTimeStamp = timeTemp;

	//update tick infos
	unsigned long clockTemp = getClock();
	lastFrameClockTicks = clockTemp - lastFrameClockStamp;
	lastFrameClockStamp = clockTemp;

	//update average frame time
	if (frameNum > 1) {
		if (averageFrameDuration <= 0){
			averageFrameDuration = (double)lastFrameDuration;
		}
		else{
			//über 100 frames.
			averageFrameDuration = averageFrameDuration * 0.99;
			averageFrameDuration = averageFrameDuration + 0.01 * (double)lastFrameDuration;

			//berechn. fps
			framesPerSec = (float)(1000.0/averageFrameDuration);
		}
	}
}

unsigned int Timing::getTime(){

	time_t nowT;
	nowT = time(0);

	//umwandlung in int
	//...

	return 0;
}

unsigned long Timing::getClock(){

	clock_t nowC;
	nowC = clock();

	//umwandlung in long
	//...

	return 0;
}
*/