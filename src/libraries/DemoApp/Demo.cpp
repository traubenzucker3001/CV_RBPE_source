
//!noch gl fehler!

// <<<<<<<<<< includes >>>>>>>>>> //
#include <iostream>

#include "Demo.h"
#include "Scene.h"
#include "PhysicEngine\UniformGrid.h"
#include "PhysicEngine\Cuda.h"
#include "PhysicEngine\World.h"
#include "PhysicEngine\RigidBody.h"

using namespace std;


void resizeCallback(GLFWwindow *window, int w, int h){

	Demo::getInstance()->camera->setWidthHeight(w, h);
	glViewport(0, 0, w, h);
}


Demo::Demo(int wwIN, int whIN, float durIN, float tvIN, float wsIN, float prIN, float scIN, float dcIN, int bnIN){

	cout << "demo: demo constr called!" << endl; //zum test

	physicsWorld = new World(wsIN,prIN,scIN,dcIN,bnIN);
	virtObjNum = 0;
	time = new Timing();
	windowWidth = wwIN;
	windowHeight = whIN;
	duration = durIN;
	terminalVeloc = tvIN;
	camera = new CVK::Trackball(wwIN,whIN);
	sceneRoot = new CVK::Node("Root");

	float temp = prIN * 6;
	geometry = new CVK::Cube(temp);
}

Demo::~Demo(){

	delete time;
	delete physicsWorld;
	delete camera;
}

void Demo::run(){

	cout << "demo: run!" << endl; //zum test

	// Init GLFW and GLEW
	glfwInit();
	CVK::useOpenGL33CoreProfile();
	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "RBPE-Demo", 0, 0);
	glfwSetWindowPos( window, 100, 50);
	glfwMakeContextCurrent(window);

	glewInit();

	//vbos generieren und binden. eins für gesamtes szene oder pro box eins??
	/*glGenBuffers(1, &rbVBO);
	glBindBuffer(GL_ARRAY_BUFFER, rbVBO);
	int bufferSize = vertexCount * 3 * sizeof(float);
	glBufferData(GL_ARRAY_BUFFER, 3 * bufferSize, vertexData, GL_DYNAMIC_DRAW);*/
	//glBufferData(GL_ARRAY_BUFFER, numParticles * 3 * sizeof(float), 0, GL_DYNAMIC_DRAW); // locate the memory, but without initialize the values  

	camera->setCenter( glm::vec3( 0.0f, 0.0f, 0.0f));
	camera->setRadius( 5);
	camera->setNearFar( 1.0f, 10.0f);

	glfwSetWindowSizeCallback( window, resizeCallback);

	//in demo mit reinpacken, keine extra scene klasse!?
	initScene();

	//load, compile and link Shader
	const char *shadernames[2] = {SHADERS_PATH "/Examples/Phong.vert", SHADERS_PATH "/Examples/Phong.frag"};
	CVK::ShaderPhong phongShader( VERTEX_SHADER_BIT|FRAGMENT_SHADER_BIT, shadernames);

	//OpenGL parameters
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//hierarchie aufbau


	CVK::State::getInstance()->setCamera( camera);

	//define light
	CVK::Light plight( glm::vec4( -1, 1, 1, 1), grey, glm::vec3( 0, 0, 0), 1.0f, 0.0f);
	CVK::State::getInstance()->addLight( &plight);

	//define Scene uniforms (ambient and fog)
	CVK::State::getInstance()->updateSceneSettings( darkgrey, 0, white, 1, 10, 1);
	CVK::State::getInstance()->setShader(&phongShader);

	//...

	//init cuda
	if(isGPU == true){
		Cuda::getInstance()->initCUDA();
	}

	//schauen wie am besten machen mit virtobjs und step simulation
	while( !glfwWindowShouldClose(window)){

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		time->startFrame();
		//display();	//überflüssig
		stepSimulation(duration);

		//Update Camera
		camera->update(window);

		//update shader and render
		phongShader.update();

		//unterschiedlich bei cpu o. gpu ausführung
		//vorher positionen und orientierung von VOs aktualisieren
		//obj rendern
		//earthNode->render();
		updateVOs();
		sceneRoot->render();

		glfwSwapBuffers(window);
		glfwPollEvents();

		time->endFrame();

		float fps = (float)time->updateFPS();
		char title[64];
		sprintf_s(title, "Rigid Body | %d fps", (int)fps);
		glfwSetWindowTitle(window, title);
	}
	Cuda::getInstance()->~Cuda();	//free cuda stuff

	glfwDestroyWindow( window);
	glfwTerminate();
}

void Demo::initScene(){

	cout << "demo: initScene called!" << endl; //zum test

	//gpu benutzt andere create grid
	if(isGPU == false){
		UniformGrid::getInstance()->createGrid();
	}

	//obj initialisieren
	//initObjs();


	/*	VOs erstellen
		- RB erstellen
		-- parts erstellen
		- CVK::Node erstellen
	*/
	cout << "scene: initObjs called!" << endl; //zum test

	//...
	//World::getInstance()->setAllPartNum(0);
	//vertexCount = 0;

	int numberRB = World::getInstance()->getAllBodyNum();
	int numberP = World::getInstance()->getAllPartNum();
	//int numberRB = World::getInstance()->allBodyNum;
	//int numberP = World::getInstance()->allPartNum;

	//VOs anlegen u. in vector listen
	float pR = World::getInstance()->getPartRadius();

	glm::vec3 randPose = glm::vec3();
	float mass = 0;		//todo: geeignete masse definieren!!!
	for (int i = 0; i < numberRB; i++){
		VirtualObject *temp = new VirtualObject(randPose,i,mass,false,false);
		virtualObjs.push_back(temp);
	}

	//----------anhang
	//boxen erstellen
	/*
	for (int i = 0; i<numberRB; i++) {
		World::getInstance()->allBodies[i] = new Box();
		numberP += 27;
		vertexCount += 24;
	}
	//todo: array in entsprechender größe erstellen, mit in world konstr. packen!!! zugriff auf array ändern?! public?!
	particles = new Particle*[numberP];

	for (int i = 0; i<numberRB; i++) {
		bodies[i]->shape->populatePartArray();
	}
	std::cout << "Number of Particles: " << numberP << std::endl;
	std::cout << "VBO vertex count: " << vertexCount << std::endl;
	*/

	//objekte initialisieren
	//teapot jetzt nur zum test, später virtual objekts
	//CVK::Teapot *teapot = new CVK::Teapot;
	//CVK::Sphere *sphere = new CVK::Sphere(0.3f);
}

//wenn nur step simulation drin bleibt, dann ja eig überflüssig
/*void Demo::display(){

	stepSimulation(timeDelta);

}*/

void Demo::stepSimulation(float duration){

	cout << "demo: step simulation!" << endl; //zum test

	World::getInstance()->stepPhysics(duration,isGPU);

}

void Demo::resetScene(){

	//...
}

void Demo::updateVOs(){

	//wenn cpu
	if (isGPU == false){
		for (std::vector<VirtualObject*>::iterator it = virtualObjs.begin(); it != virtualObjs.end(); ++it){
			(*it)->updateCPU();
		}
	}
	//wenn gpu
	else{
		for (std::vector<VirtualObject*>::iterator it = virtualObjs.begin(); it != virtualObjs.end(); ++it){
			(*it)->updateGPU();
		}
	}
}