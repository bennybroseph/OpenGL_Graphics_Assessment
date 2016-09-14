#include "MyApplication.h"

#include <fstream>

#include "Shader.h"
#include "FlyCamera.h"
#include "Gizmos.h"

using std::fstream;

const static int GRID_SIZE = 10;
const static int GRID_SEPARATOR = 5;

const static unsigned int VERTEX_SIZE = 4;
const static unsigned int INDEX_BUFFER_SIZE = 4;

int MyApplication::startup()
{
	createWindow("Intro to OpenGL", 1280, 720);

	Shader::init();
	Gizmos::init();

	//setup some camera stuff
	m_camera = new FlyCamera();

	m_sun = Planet(vec3(0, 0, 0), 1.5f, vec4(255.f / 255.f, 235.f / 255.f, 59.f / 255.f, 1.f), 1.5f);

	m_earth = Planet(vec3(5, 0, 0), 0.5f, vec4(139 / 255.f, 195 / 255.f, 74 / 255.f, 1.f), 3.f);
	m_earth.transform().setParent(&m_sun.transform());

	m_moon = Planet(vec3(2, 0.5f, 0), 0.3f, vec4(0.9f, 0.9f, 0.9f, 1.f), 5.f);
	m_moon.transform().setParent(&m_earth.transform());

	/*auto newSphere = new Sphere();
	newSphere->shader() = Shader::positionShader();
	newSphere->shouldDrawWireFrame() = true;
	newSphere->transform().setLocalPosition(vec3(0.f, 0.5f, 0.f));
	m_shapes.push_back(newSphere);

	auto newPlane = new Plane();
	newPlane->transform().scale(vec3(5.f, 1.f, 5.f));
	m_shapes.push_back(newPlane);

	auto newCube = new Cube();
	newCube->transform().setLocalPosition(vec3(1.f, 0.5f, 0.f));
	m_shapes.push_back(newCube);*/

	return true;
}

void MyApplication::shutdown()
{
	Gizmos::quit();
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
		m_shapes[0]->transform().scale(vec3(1.f + m_deltaTime, 1.f + m_deltaTime, 1.f + m_deltaTime));

	if (Input::getKey(GLFW_KEY_COMMA, GLFW_MOD_SHIFT) >= GLFW_PRESS)
		m_shapes[0]->transform().scale(vec3(1.f - m_deltaTime, 1.f - m_deltaTime, 1.f - m_deltaTime));
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
	// clear the screen for this frame
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (m_shouldDrawGrid)
	{
		for (auto shape : m_shapes)
			shape->draw();
	}

	drawSolarSystem();
}

void MyApplication::drawSolarSystem()
{
	Gizmos::drawSphere(m_sun.transform().getWorldSpaceMatrix(), m_sun.colour(), true);
	Gizmos::drawSphere(m_earth.transform().getWorldSpaceMatrix(), m_earth.colour(), true);
	Gizmos::drawSphere(m_moon.transform().getWorldSpaceMatrix(), m_moon.colour(), true);
}

MyApplication::~MyApplication()
{
	for (auto shape : m_shapes)
		delete shape;
}
