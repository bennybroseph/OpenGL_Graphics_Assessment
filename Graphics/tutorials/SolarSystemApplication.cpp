#include "SolarSystemApplication.h"
#include "gl_core_4_4.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "Camera.h"
#include "Gizmos.h"

using glm::vec3;
using glm::vec4;
using glm::mat4;

SolarSystemApplication::SolarSystemApplication() 
	: m_camera(nullptr),
	m_direction(false) {

}

SolarSystemApplication::~SolarSystemApplication() {

}

bool SolarSystemApplication::startup() {

	// create a basic window
	createWindow("AIE OpenGL Application", 1280, 720);

	// start the gizmo system that can draw basic immediate-mode shapes
	Gizmos::create();

	// setup camera
	m_camera = new Camera(glm::pi<float>() * 0.25f, 16 / 9.f, 0.1f, 1000.f);
	m_camera->setLookAtFrom(vec3(10, 10, 10), vec3(0));

	// set input callback
	setInputCallback(inputCallback);
	
	return true;
}

void SolarSystemApplication::shutdown() {

	// cleanup camera and gizmos
	delete m_camera;
	Gizmos::destroy();

	// destroy our window properly
	destroyWindow();
}

bool SolarSystemApplication::update(float deltaTime) {
	
	// close the application if the window closes or we press escape
	if (glfwWindowShouldClose(m_window) ||
		glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		return false;

	// update the camera's movement
	m_camera->update(deltaTime);

	// clear the gizmos out for this frame
	Gizmos::clear();

	vec4 white(1, 1, 1, 0);
	vec4 black(0, 0, 0, 1);

	// for now let's add a grid to the gizmos
	for (int i = 0; i < 21; ++i) {
		Gizmos::addLine(vec3(-10 + i, 0, 10), vec3(-10 + i, 0, -10),
			i == 10 ? white : black);
		Gizmos::addLine(vec3(10, 0, -10 + i), vec3(-10, 0, -10 + i),
			i == 10 ? white : black);
	}

	Gizmos::addTransform(mat4(1));

	float time = (float)glfwGetTime();

	// simple solar system
	
	// sun spins (reverses if direction says to)
	mat4 sun = glm::rotate(time * 0.1f * (m_direction ? -1 : 1), vec3(0, 1, 0));

	// earth spins around sun
	mat4 earth = sun * glm::translate(vec3(8,0,0)) * glm::rotate(time, vec3(0,1,0));

	// moon spins around earth but always facing the earth the same way
	mat4 moon = earth * glm::rotate(time * 1.5f, vec3(0, 1, 0)) * glm::translate(vec3(1.5f, 0, 0));
	
	// add simple spheres for each
	Gizmos::addSphere(vec3(sun[3]), 2, 16, 16, vec4(1, 1, 0, 1), &sun);
	Gizmos::addSphere(vec3(earth[3]), 0.5f, 16, 16, vec4(0, 0, 1, 1), &earth);
	Gizmos::addSphere(vec3(moon[3]), 0.125f, 16, 16, vec4(0.6f, 0.6f, 0.6f, 1), &moon);

	// lines and transforms show how the matrices are relative
	Gizmos::addLine(vec3(sun[3]), vec3(earth[3]), vec4(1, 0, 0, 1));
	Gizmos::addLine(vec3(earth[3]), vec3(moon[3]), vec4(1, 0, 0, 1));
	Gizmos::addTransform(sun, 3);
	Gizmos::addTransform(earth, 1);
	Gizmos::addTransform(moon, 0.5f);

	// return true, else the application closes
	return true;
}

void SolarSystemApplication::draw() {

	// clear the screen for this frame
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// display the 3D gizmos
	Gizmos::draw(m_camera->getProjectionView());

	// get a orthographic projection matrix and draw 2D gizmos
	int width = 0, height = 0;
	glfwGetWindowSize(m_window, &width, &height);
	mat4 guiMatrix = glm::ortho<float>(0, 0, (float)width, (float)height);

	Gizmos::draw2D(guiMatrix);
}

void SolarSystemApplication::inputCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {

	SolarSystemApplication* myThis = (SolarSystemApplication*)glfwGetWindowUserPointer(window);

	if (key == GLFW_KEY_SPACE)
		if (action == GLFW_PRESS)
			myThis->m_direction = true;
		else if (action == GLFW_RELEASE)
			myThis->m_direction = false;
}