//szenen-erstellung ausgelagert
//k�nnte man eig auch mit in main packen

#include <iostream>
#include "Scene.h"
#include "CVK_Framework\CVK_Framework.h"

using namespace std;

//methode zur erstellung der szene
static void initScene(){

	//...

	//objekte initialisieren
	//teapot jetzt nur zum test, sp�ter virtual objekts
	CVK::Teapot *teapot = new CVK::Teapot;
	CVK::Sphere *sphere = new CVK::Sphere(0.3f);
}

