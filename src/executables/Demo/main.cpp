#include <iostream>
#include "CVK_Framework\CVK_Framework.h"

#include <cuda.h>
#include <cuda_runtime.h>

#include "DemoApp\Demo.h"

using namespace std;

#define WIDTH 800
#define HEIGHT 800

//simulations parameter
Demo *demo;
float dur = 0.01f;
float termVeloc = 20.0f;
float springCoeff = 100.0f;
float dampCoeff = 0.5f;		
bool iGPU = false;			//false für cpu, true für gpu
bool iGrid = true;
bool renderP = false;		//momentan nur bei cpu vers möglich
float worldSize = 15.0f;	//Half length of world	//15
float partRadius = 0.40f;
float boxHSize = partRadius * 3;
int bodyNumber = 100;

int main() {

	demo = new Demo(WIDTH, HEIGHT, dur, termVeloc, worldSize, partRadius, springCoeff, dampCoeff,bodyNumber, iGPU, iGrid, renderP);
	demo->run();

	cudaDeviceReset();
	return 0;
}
