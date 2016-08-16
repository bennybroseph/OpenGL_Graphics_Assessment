#include "MyApplication.h"
#include <Gizmos.h>

using glm::vec3;
using glm::vec4;
using glm::mat4;

const static int GRID_SIZE = 10;
const static int GRID_SEPARATOR = 5;

int MyApplication::startup()
{
	createWindow("Intro to OpenGL", 1280, 720);

	Gizmos::create();

	//setup some camera stuff
	//to do
	m_view = lookAt(vec3(10, 10, 10), vec3(0), vec3(0, 1, 0));
	m_projection = glm::perspective(glm::pi<float>() * 0.25f, 16 / 9.f, 0.1f, 1000.f);

	m_sun = mat4(1.0f);
	m_earth = mat4(1.0f);
	m_earth[3] = vec4(0, 5, 0, 1);
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

void MyApplication::parseInput()
{
	// close the application if the window closes or we press escape
	if (glfwWindowShouldClose(m_window) || glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		m_isRunning = false;
		return;
	}

	if (glfwGetKey(m_window, GLFW_KEY_F1) == GLFW_PRESS && m_prevF1State != GLFW_PRESS)
		m_shouldDrawGrid = !m_shouldDrawGrid;

	if (glfwGetKey(m_window, GLFW_KEY_1) == GLFW_PRESS )
	{
		m_sun = translate(m_sun, vec3(0, 0.02f, 0));
	}

	if (glfwGetKey(m_window, GLFW_KEY_2) == GLFW_PRESS)
	{
		m_earth = translate(m_earth, vec3(0.02f, 0, 0));
	}

	m_prevF1State = glfwGetKey(m_window, GLFW_KEY_F1);
}

void MyApplication::update()
{

}

void MyApplication::lateUpdate()
{

}

void MyApplication::draw()
{
	Gizmos::clear();

	if (m_shouldDrawGrid)
		drawGrid();

	drawSolarSystem();

	// clear the screen for this frame
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Gizmos::draw(m_projection * m_view);
}
float rotinc = 5;
void MyApplication::drawSolarSystem()
{
	printf("%f \n", m_deltaTime);
	rotinc += m_deltaTime;
	//m_sun = rotate(m_sun, rotinc * -1.f  , vec3(0, 1, 0));	

	Gizmos::addSphere(vec3(m_sun[3]), 1.f, 25, 25, vec4(0, 0, 0, 1), &m_sun);	
	Gizmos::addSphere(vec3(m_earth[3]) + vec3(m_sun[3]), 1.f, 25, 25, vec4(0, 0, 0, 1), &m_earth);	
}

void MyApplication::drawGrid()
{
	vec4 white(1);
	vec4 black(0, 0, 0, 1);

	for (auto i = 0; i <= GRID_SIZE * 2; ++i)
	{
		Gizmos::addLine(
			vec3(-GRID_SIZE + i, 0, GRID_SIZE),
			vec3(-GRID_SIZE + i, 0, -GRID_SIZE),
			(i - GRID_SIZE) % GRID_SEPARATOR == 0 ? white : black);
		Gizmos::addLine(
			vec3(GRID_SIZE, 0, -GRID_SIZE + i),
			vec3(-GRID_SIZE, 0, -GRID_SIZE + i),
			(i - GRID_SIZE) % GRID_SEPARATOR == 0 ? white : black);
	}

	Gizmos::addTransform(mat4(1));
}
