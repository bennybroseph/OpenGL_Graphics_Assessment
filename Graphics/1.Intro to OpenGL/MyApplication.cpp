#include "MyApplication.h"

#include <fstream>

#include <Gizmos.h>

#include "Shader.h"
#include "FlyCamera.h"
#include "Plane.h"
#include "Cube.h"

using std::fstream;

const static int GRID_SIZE = 10;
const static int GRID_SEPARATOR = 5;

const static unsigned int VERTEX_SIZE = 4;
const static unsigned int INDEX_BUFFER_SIZE = 4;

int MyApplication::startup()
{
	createWindow("Intro to OpenGL", 1280, 720);

	Shader::init();
	Plane::init();
	Cube::init();

	Gizmos::create();

	//setup some camera stuff
	m_camera = new FlyCamera();

	m_sun = Planet(vec3(0, 0, 0), 1.5f, vec4(255.f / 255.f, 235.f / 255.f, 59.f / 255.f, 0.75f), 1.5f);

	m_earth = Planet(vec3(5, 0, 0), 1.f, vec4(139 / 255.f, 195 / 255.f, 74 / 255.f, 0.75f), 3.f);
	m_earth.transform().setParent(&m_sun.transform());

	m_moon = Planet(vec3(2, 0.5f, 0), 0.3f, vec4(0.9f, 0.9f, 0.9f, 1), 5.f);
	m_moon.transform().setParent(&m_earth.transform());

	/*auto newPlane = new Plane();
	newPlane->transform().scale(vec3(5.f, 1.f, 5.f));

	m_shapes.push_back(newPlane);*/

	auto newCube = new Cube();

	m_shapes.push_back(newCube);

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
	if (glfwWindowShouldClose(m_window) || Input::getKey(GLFW_KEY_ESCAPE) >= GLFW_PRESS)
	{
		m_isRunning = false;
		return;
	}

	if (Input::getKey(GLFW_KEY_F1) == GLFW_PRESS)
		m_shouldDrawGrid = !m_shouldDrawGrid;

	if (Input::getKey(GLFW_KEY_1) >= GLFW_PRESS)
		m_sun.transform().translate(vec3(0.f, 1.f * m_deltaTime, 0.f));

	if (Input::getKey(GLFW_KEY_2) >= GLFW_PRESS)
		m_earth.transform().translate(vec3(0.f, 1.f * m_deltaTime, 0.f));

	if (Input::getKey(GLFW_KEY_3) >= GLFW_PRESS)
		m_moon.transform().setPosition(vec3(0, 5, 0));

	if (Input::getKey(GLFW_KEY_PERIOD, GLFW_MOD_SHIFT) >= GLFW_PRESS)
		for (auto shape : m_shapes)
			shape->transform().scale(vec3(1.f + m_deltaTime, 1.f + m_deltaTime, 1.f + m_deltaTime));

	if (Input::getKey(GLFW_KEY_COMMA, GLFW_MOD_SHIFT) >= GLFW_PRESS)
		for (auto shape : m_shapes)
			shape->transform().scale(vec3(1.f - m_deltaTime, 1.f - m_deltaTime, 1.f - m_deltaTime));
}

void MyApplication::update()
{
	m_sun.update(m_deltaTime);
	m_earth.update(m_deltaTime);
	m_moon.update(m_deltaTime);

	m_camera->update(m_deltaTime);
}

void MyApplication::lateUpdate() { }

void MyApplication::draw()
{
	Gizmos::clear();

	// clear the screen for this frame
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (m_shouldDrawGrid)
	{
		for (auto shape : m_shapes)
			shape->draw();
	}

	drawSolarSystem();

	Gizmos::draw(m_camera->getProjectionView());
}

void MyApplication::drawSolarSystem()
{
	/*Gizmos::addSphere(
		m_sun.transform().getPosition(),
		m_sun.radius(), 25, 25,
		m_sun.colour(),
		&m_sun.transform().getWorldSpaceMatrix());*/

	Gizmos::addSphere(
		m_earth.transform().getPosition(),
		m_earth.radius(), 25, 25,
		m_earth.colour(),
		&m_earth.transform().getWorldSpaceMatrix());

	Gizmos::addSphere(
		m_moon.transform().getPosition(),
		m_moon.radius(), 25, 25,
		m_moon.colour(),
		&m_moon.transform().getWorldSpaceMatrix());
}

MyApplication::~MyApplication()
{
	for (auto shape : m_shapes)
		delete shape;
}
