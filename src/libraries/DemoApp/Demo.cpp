#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "Demo.h"
#include "Scene.h"

#include <glm\glm.hpp>
#include <GL/glew.h>


Demo::Demo(){

	//camera
	//von camera zu trackball muckt noch rum, aber nur trackball auch
	camera = CVK::Trackball(windowWidth, windowHeight);
}

void Demo::initPhysics(){

	//neue engine erstellt
}

void Demo::initGraphics(){

	//init cvk

	//CVK::Trackball cam = camera;

	//init GLFW, GLEW
	glfwInit();
	CVK::useOpenGL33CoreProfile();
	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "RBPE-Demo", 0, 0);
	glfwSetWindowPos(window, 100, 50);
	glfwMakeContextCurrent(window);

	glewInit();

	camera.setCenter(glm::vec3(0.0f, 0.0f, 0.0f));
	camera.setRadius(5);
	camera.setNearFar(1.0f, 10.0f);

	//callback muss noch irgwie übergangen werden Demo::... nimmt er nicht
	glfwSetWindowSizeCallback(window, resizeCallback);

	initScene();
	//war erst mit bei main, da konnta aber nicht drauf zugegriffen werden

	//load, compile and link Shader
	//noch durchsehen was für shader ich überhaupt brauche
	const char *shadernames[2] = { SHADERS_PATH "/Examples/Phong.vert", SHADERS_PATH "/Examples/Phong.frag" };
	CVK::ShaderPhong phongShader(VERTEX_SHADER_BIT | FRAGMENT_SHADER_BIT, shadernames);

	//materialien definieren
	//mal sehen ob nötig
	CVK::Material mat_brick((char*)RESOURCES_PATH "/brick.bmp", black, darkgrey, 100.0f);
	CVK::Material mat_cvlogo((char*)RESOURCES_PATH "/cv_logo.bmp", black, grey, 100.0f);

	//OpenGL parameters
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//möglicherweise hierarchie aufbau
	//...

	//set kamera
	CVK::State::getInstance()->setCamera(&camera);

	//define light
	CVK::Light plight(glm::vec4(-1, 1, 1, 1), grey, glm::vec3(0, 0, 0), 1.0f, 0.0f);
	CVK::State::getInstance()->addLight(&plight);

	//define Scene uniforms (ambient and fog)
	CVK::State::getInstance()->updateSceneSettings(darkgrey, 0, white, 1, 10, 1);
	CVK::State::getInstance()->setShader(&phongShader);

	double startTime = glfwGetTime();

	while (!glfwWindowShouldClose(window)){
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//delta time
		float deltaT = glfwGetTime() - startTime;
		startTime = glfwGetTime();

		//rotate in relation to time
		//hier kommt dann der physik ablauf rein
		/*
		earthNode->setModelMatrix(glm::rotate(*earthNode->getModelMatrix(), 10.0f * deltaT, glm::vec3(0.0, 1.0, 0.0)));
		moonNode->setModelMatrix(glm::rotate(*moonNode->getModelMatrix(), 40.0f * deltaT, glm::vec3(1.0, 1.0, 1.0)));
		*/

		//Update Camera
		camera.update(window);

		//update shader and render
		phongShader.update();
		//hierarchie bzw. alle obj. rendern
		//earthNode->render();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();
}

void Demo::display(){
}

void Demo::update(){
}

// <<<<<<<<<< callback funcs >>>>>>>>>> //
void Demo::keyCallback(){
}

void Demo::resizeCallback(GLFWwindow *window, int w, int h){

	camera.setWidthHeight(w, h);
	glViewport(0, 0, w, h);
}

void Demo::mouseButtonCallback(){
}

void Demo::mouseMotionCallback(){
}

