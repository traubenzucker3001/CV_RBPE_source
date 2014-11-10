//szenen-erstellung ausgelagert
//könnte man eig auch mit in main packen

#include <iostream>
#include "Scene.h"
#include "CVK_Framework\CVK_Framework.h"
#include "PhysicEngine\World.h"
#include "PhysicEngine\RigidBody.h"

using namespace std;

//methode zur erstellung (zum füllen) der szene
static void initObjs(){

	//...
	World::getInstance()->setAllPartNum(0);
	vertexCount = 0;

	int numberRB = World::getInstance()->getAllBodyNum();
	int numberP = World::getInstance()->getAllPartNum();

	//todo: array in entsprechender größe erstellen, mit in world konstr. packen!!!
	RigidBody *bodies = new RigidBody[numberRB];
	World::getInstance()->setAllBodies(&bodies);

	//boxen erstellen
	for (int i = 0; i<numberRB; i++) {
		bodies[i] = new Box();
		numberP += 27;
		vertexCount += 24;
	}
	//todo: array in entsprechender größe erstellen, mit in world konstr. packen!!! zugriff auf array ändern?! public?!
	particles = new Particle*[numberP];

	for (int i = 0; i<numberRB; i++) {
		bodies[i]->populateParticleArray();
	}
	std::cout << "Number of Particles: " << numberP << std::endl;
	std::cout << "VBO vertex count: " << vertexCount << std::endl;

	//objekte initialisieren
	//teapot jetzt nur zum test, später virtual objekts
	CVK::Teapot *teapot = new CVK::Teapot;
	CVK::Sphere *sphere = new CVK::Sphere(0.3f);
}

