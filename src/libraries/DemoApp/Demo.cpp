
//!noch gl fehler!

// <<<<<<<<<< includes >>>>>>>>>> //
#include "Demo.h"
#include "Scene.h"
#include "PhysicEngine/UniformGrid.h"
#include "PhysicEngine/Cuda.h"

Demo::Demo(int wwIN, int whIN, float durIN, float tvIN, float wsIN, float prIN, float scIN, float dcIN){

	physicsWorld = new World(wsIN,prIN,scIN,dcIN);
	virtObjNum = 0;
	time = new Timing();
	windowWidth = wwIN;
	windowHeight = whIN;
	duration = durIN;
	terminalVeloc = tvIN;
}

Demo::~Demo(){

	//...
}

void Demo::run(){

	// Init GLFW and GLEW
	glfwInit();
	CVK::useOpenGL33CoreProfile();
	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "RBPE-Demo", 0, 0);
	glfwSetWindowPos( window, 100, 50);
	glfwMakeContextCurrent(window);

	glewInit();

	camera.setCenter( glm::vec3( 0.0f, 0.0f, 0.0f));
	camera.setRadius( 5);
	camera.setNearFar( 1.0f, 10.0f);

	glfwSetWindowSizeCallback( window, resizeCallback);

	initScene();

	//load, compile and link Shader
	const char *shadernames[2] = {SHADERS_PATH "/Examples/Phong.vert", SHADERS_PATH "/Examples/Phong.frag"};
	CVK::ShaderPhong phongShader( VERTEX_SHADER_BIT|FRAGMENT_SHADER_BIT, shadernames);

	//OpenGL parameters
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	CVK::State::getInstance()->setCamera( &camera);

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
		camera.update(window);

		//update shader and render
		phongShader.update();

		//unterschiedlich bei cpu o. gpu ausführung
		//vorher positionen und orientierung von VOs aktualisieren
		//obj rendern
		//earthNode->render();

		glfwSwapBuffers(window);
		glfwPollEvents();

		time->endFrame();

		float fps = (float)time->updateFPS();
		char title[64];
		sprintf(title, "Rigid Body | %d fps", (int)fps);
		glfwSetWindowTitle(window, title);
	}
	glfwDestroyWindow( window);
	glfwTerminate();
}

void Demo::initScene(){

	//gpu benutzt andere create grid
	if(isGPU == false){
		UniformGrid::getInstance()->createGrid();
	}

	//obj initialisieren
	initObjs();
}

//wenn nur step simulation drin bleibt, dann ja eig überflüssig
/*void Demo::display(){

	stepSimulation(timeDelta);

}*/

void Demo::stepSimulation(float duration){

	World::getInstance()->stepPhysics(duration,isGPU);

	/*
	//update part. values
	for(std::vector<VirtualObject*>::iterator it = virtualObjs.begin(); it != virtualObjs.end(); ++it){
		(*it)->updatePartValuesVO();
	}

	//update grid
	updateGrid();

	//update momenta
	for(std::vector<VirtualObject*>::iterator it = virtualObjs.begin(); it != virtualObjs.end(); ++it){
		(*it)->updateMomentaVO(duration);
	}

	//iterate
	for(std::vector<VirtualObject*>::iterator it = virtualObjs.begin(); it != virtualObjs.end(); ++it){
		(*it)->iterateVO(duration);
	}
	*/
}

void Demo::resetScene(){

	//...
}
