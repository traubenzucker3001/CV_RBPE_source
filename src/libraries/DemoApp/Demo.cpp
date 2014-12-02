
// <<<<<<<<<< includes >>>>>>>>>> //
#include <iostream>

#include "Demo.h"
//#include "Scene.h"
#include "PhysicEngine\UniformGrid.h"
#include "PhysicEngine\Cuda.h"
#include "PhysicEngine\World.h"
#include "PhysicEngine\RigidBody.h"

using namespace std;

//link fix try 4
extern Demo* demo;
World* world;
extern Cuda* cuda;
extern int bodycount;

void resizeCallback(GLFWwindow *window, int w, int h){

	demo->camera->setWidthHeight(w, h);
	glViewport(0, 0, w, h);
}


Demo::Demo(int wwIN, int whIN, float durIN, float tvIN, float wsIN, float prIN, float scIN, float dcIN, int bnIN, bool igIN){

	cout << "demo: demo constr called!" << endl; //zum test

	//physicsWorld = new World(wsIN,prIN,scIN,dcIN,bnIN);
	world = new World(wsIN, prIN, scIN, dcIN, bnIN);
	virtObjNum = 0;
	time = new Timing();
	windowWidth = wwIN;
	windowHeight = whIN;
	duration = durIN;
	terminalVeloc = tvIN;
	camera = new CVK::Trackball(wwIN,whIN);
	sceneRoot = new CVK::Node("Root");

	float temp = prIN * 6;
	//geometry = new CVK::Cube(temp);
	geometry = 0;
	plane = 0;
	isGPU = igIN;
}

Demo::~Demo(){

	delete time;
	//delete physicsWorld;
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

	//zum debuggen: aus konstruktor gepackt
	float pr = world->getPartRadius();
	float temp = pr * 6;
	geometry = new CVK::Cube(temp);
	//material setzten, geht aber nur bei node, also in VO
	
	//plane für boden
	plane = new CVK::Plane();
	CVK::Node* planeNode = new CVK::Node("Plane");
	CVK::Material mat_brick((char*)RESOURCES_PATH "/brick.bmp", black, darkgrey, 100.0f);
	planeNode->setGeometry(plane);
	planeNode->setMaterial(&mat_brick);
	//planeNode->setModelMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0.72, 0)));
	planeNode->setModelMatrix(glm::rotate(glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(0, -2, 0)), glm::vec3(7)), -90.0f, glm::vec3(1, 0, 0)));
	demo->sceneRoot->addChild(planeNode);

	//zum test
	/*
	CVK::Teapot *teapot = new CVK::Teapot;
	CVK::Material mat_cvlogo((char*)RESOURCES_PATH "/cv_logo.bmp", black, grey, 100.0f);
	sceneRoot->setGeometry(teapot);
	sceneRoot->setMaterial(&mat_cvlogo); 
	*/

	camera->setCenter( glm::vec3( 0.0f, 0.0f, 0.0f));
	camera->setRadius( 20);
	camera->setNearFar( 1.0f, 30.0f);

	glfwSetWindowSizeCallback( window, resizeCallback);

	initScene();

	//load, compile and link Shader
	const char *shadernames[2] = {SHADERS_PATH "/Phong.vert", SHADERS_PATH "/Phong.frag"};
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
		cuda->initCUDA();
	}

	//schauen wie am besten machen mit virtobjs und step simulation
	while( !glfwWindowShouldClose(window)){

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		time->startFrame();

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
	cuda->~Cuda();	//free cuda stuff

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
	cout << "demo: initObjs called!" << endl; //zum test

	//...
	//World::getInstance()->setAllPartNum(0);
	//vertexCount = 0;

	int numberRB = world->getAllBodyNum();
	int numberP = world->getAllPartNum();
	//int numberRB = World::getInstance()->allBodyNum;
	//int numberP = World::getInstance()->allPartNum;

	//VOs anlegen u. in vector listen
	float pR = world->getPartRadius();

	//glm::vec3 randPose = glm::vec3();
	for (int i = 0; i < numberRB; i++){
		float hSize = pR * 3;
		float x, y, z;
		x = (bodycount % 2) * 1.9f * hSize;
		y = bodycount * 3.0f * hSize;
		z = ((bodycount % 4) / 2) * 1.9f * hSize;
		glm::vec3 randPos = glm::vec3(x,y,z);


		float mass = 0.2f;
	//for (int i = 0; i < numberRB; i++){
		VirtualObject *temp = new VirtualObject(randPos,i,mass,false,false,hSize,i);
		virtualObjs.push_back(temp);
	}

	for (int i = 0; i<numberRB; i++) {
		world->allBodies[i]->shape->populatePartArray();
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

	*/
}

void Demo::stepSimulation(float duration){

	cout << "demo: step simulation!" << endl; //zum test

	world->stepPhysics(duration, isGPU);

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
		cuda->updateVOarrays();		//get current pos and rot values from gpu

		for (std::vector<VirtualObject*>::iterator it = virtualObjs.begin(); it != virtualObjs.end(); ++it){
			(*it)->updateGPU();
		}
	}
}