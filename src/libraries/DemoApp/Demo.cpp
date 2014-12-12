
// <<<<<<<<<< includes >>>>>>>>>> //
#include <iostream>

#include "Demo.h"
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

	//cout << "demo: demo constr called!" << endl; //zum test

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
	geometry = 0;
	plane = 0;
	isGPU = igIN;
	cubeMaterial = 0;
}

Demo::~Demo(){

	delete time;
	//delete physicsWorld;
	delete camera;
}

void Demo::run(){

	//cout << "demo: run!" << endl; //zum test

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
	cubeMaterial = new CVK::Material((char*)RESOURCES_PATH "/cv_logo.bmp", black, grey, 100.0f);

	//plane für boden
	plane = new CVK::Plane();
	CVK::Node* planeNode = new CVK::Node("Plane");
	CVK::Material mat_brick((char*)RESOURCES_PATH "/brick.bmp", black, darkgrey, 100.0f);
	planeNode->setGeometry(plane);
	planeNode->setMaterial(&mat_brick);
	//planeNode->setModelMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0.72, 0)));
	planeNode->setModelMatrix(glm::rotate(glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(0, -0.6, 0)), glm::vec3(10)), -90.0f, glm::vec3(1, 0, 0)));	//0.4=partdurchmesser
	demo->sceneRoot->addChild(planeNode);

	camera->setCenter( glm::vec3( 0.0f, 0.0f, 0.0f));
	camera->setRadius( 30);
	camera->setNearFar( 1.0f, 100.0f);

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

	while( !glfwWindowShouldClose(window)){

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		time->startFrame();

		stepSimulation(duration);

		//Update Camera
		camera->update(window);

		//update shader and render
		phongShader.update();

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

	//cout << "demo: initScene called!" << endl; //zum test

	//gpu benutzt andere create grid
	if(isGPU == false){
		UniformGrid::getInstance()->createGrid();
	}

	//cout << "demo: initObjs called!" << endl; //zum test

	int numberRB = world->getAllBodyNum();
	int numberP = world->getAllPartNum();

	//VOs anlegen u. in vector listen
	float pR = world->getPartRadius();
	
	for (int i = 0; i < numberRB; i++){
		float hSize = pR * 3;
		
		float x, y, z;
		x = (bodycount % 2) * 1.9f * hSize;			//1.9
		y = bodycount * 6.0f * hSize;				//3.0
		z = ((bodycount % 4) / 2) * 1.9f * hSize;	//1.9
		glm::vec3 randPos = glm::vec3(x,y,z);
		
		/*
		glm::vec3 randPos;
		glm::vec3 randPos1 = glm::vec3(0.0, 6.0, 0.0);
		glm::vec3 randPos2 = glm::vec3(1.0, 6.0, 0.0);
		glm::vec3 randPos3 = glm::vec3(0.0, 6.0, 0.0);
		//cout << "vopos: " << randPos.x << ", " << randPos.y << ", " << randPos.z << endl;	//zum debuggen
		if (i == 0){
			randPos = randPos1;
		}
		if (i == 1){
			randPos = randPos2;
		}
		if (i == 2){
			randPos = randPos3;
		}
		*/

		float mass = 0.2f;

		VirtualObject *temp = new VirtualObject(randPos,i,mass,false,false,hSize,i);
		virtualObjs.push_back(temp);
	}

	for (int i = 0; i<numberRB; i++) {
		world->allBodies[i]->shape->populatePartArray();
	}
	//TODO alle part pos ausgeben lassen
	/*
	int temp = world->getAllPartNum();
	for (int i = 0; i < temp; i++){
		glm::vec3 pos = world->allParticles[i]->getPosition();
		//glm::vec3 grid = world->allParticles[i]->getGridIndex();
		cout << "partpos " << i << ": " << pos.x << ", " << pos.y << ", " << pos.z << endl;	//zum debuggen
		//cout << "partgrid " << i << ": " << grid.x << ", " << grid.y << ", " << grid.z << endl;	//zum debuggen
	}
	*/
}

void Demo::stepSimulation(float duration){

	//cout << "demo: step simulation!" << endl; //zum test

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