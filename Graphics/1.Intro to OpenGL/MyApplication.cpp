#include "MyApplication.h"

using glm::vec3;
using glm::vec4;
using glm::mat4;


bool MyApplication::startup()
{
	createWindow("Intro to OpenGL", 1280, 720);

	Gizmos::create();

	//setup some camera stuff
	//to do
	m_view = glm::lookAt(vec3(10, 10, 10), vec3(0), vec3(0, 1, 0));
	m_projection = glm::perspective(glm::pi<float>() * 0.25f, 16 / 9.f, 0.1f, 1000.f);

	//setinputcallback
	//to do

	return true;
}

void MyApplication::shutdown()
{
	//delete the camera
	Gizmos::destroy(); //cleanup the gizmos
	destroyWindow();
}

bool MyApplication::update(float deltaTime)
{
	// close the application if the window closes or we press escape
	if (glfwWindowShouldClose(m_window) || glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		return false;

	Gizmos::clear();	
	
	vec4 white(1);
	vec4 black(0, 0, 0, 1);
	
	for (int i = 0; i < 21; ++i)
	{
		Gizmos::addLine(vec3(-10 + i, 0, 10), vec3(-10 + i, 0, -10), i == 10 ? white : black);
		Gizmos::addLine(vec3(10, 0, -10 + i), vec3(-10, 0, -10 + i), i == 10 ? white : black);
	}

	Gizmos::addTransform(glm::mat4(1));

	return true;
}

void MyApplication::draw()
{
	// clear the screen for this frame
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Gizmos::draw(m_projection * m_view);
}
