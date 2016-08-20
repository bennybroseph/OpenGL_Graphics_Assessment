#include "MyApplication.h"

#include <Gizmos.h>

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

	m_sun = Planet(vec3(0, 0, 0), 1.5f, vec4(255.f / 255.f, 235.f / 255.f, 59.f / 255.f, 1.f));

	m_earth = Planet(vec3(5, 0, 0), 1.f, vec4(139 / 255.f, 195 / 255.f, 74 / 255.f, 1.f), 0.01f);
	m_earth.getTransform().setParent(&m_sun.getTransform());

	m_moon = Planet(vec3(2, 0.5f, 0), 0.3f, vec4(0.9f, 0.9f, 0.9f, 1), 0.1f);
	m_moon.getTransform().setParent(&m_earth.getTransform());


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

	if (glfwGetKey(m_window, GLFW_KEY_1) == GLFW_PRESS)
	{
		m_sun.getTransform().setPosition(
			vec3(
				m_sun.getTransform().getPosition().x,
				m_sun.getTransform().getPosition().y + 0.02f,
				m_sun.getTransform().getPosition().z));
	}

	if (glfwGetKey(m_window, GLFW_KEY_2) == GLFW_PRESS)
	{
		m_earth.getTransform().setLocalPosition(
			vec3(
				m_earth.getTransform().getLocalPosition().x + 0.02f,
				m_earth.getTransform().getLocalPosition().y,
				m_earth.getTransform().getLocalPosition().z));
	}

	if (glfwGetKey(m_window, GLFW_KEY_3) == GLFW_PRESS)
	{
		m_moon.getTransform().setPosition(vec3(0, 5, 0));
	}

	m_prevF1State = glfwGetKey(m_window, GLFW_KEY_F1);
}

void MyApplication::update()
{
	m_sun.update();
	m_earth.update();
	m_moon.update();
}

void MyApplication::lateUpdate()
{
	m_sun.lateUpdate();
	m_earth.lateUpdate();
	m_moon.lateUpdate();
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

	Gizmos::addSphere(m_sun.getTransform().getPosition(), m_sun.radius(), 25, 25, m_sun.color(), &m_sun.getTransform().getMatrix());
	Gizmos::addSphere(m_earth.getTransform().getPosition(), m_earth.radius(), 25, 25, m_earth.color(), &m_earth.getTransform().getMatrix());
	Gizmos::addSphere(m_moon.getTransform().getPosition(), m_moon.radius(), 25, 25, m_moon.color(), &m_moon.getTransform().getMatrix());
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
