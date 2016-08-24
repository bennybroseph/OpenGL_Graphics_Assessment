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

	m_sun = Planet(vec3(0, 0, 0), 1.5f, vec4(255.f / 255.f, 235.f / 255.f, 59.f / 255.f, 1.f), 1.5f);

	m_earth = Planet(vec3(5, 0, 0), 1.f, vec4(139 / 255.f, 195 / 255.f, 74 / 255.f, 1.f), 3.f);
	m_earth.transform().setParent(&m_sun.transform());

	m_moon = Planet(vec3(2, 0.5f, 0), 0.3f, vec4(0.9f, 0.9f, 0.9f, 1), 5.f);
	m_moon.transform().setParent(&m_earth.transform());

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

	if (Input::getMouseButton(GLFW_MOUSE_BUTTON_1) >= GLFW_PRESS)
	{
		m_view = inverse(
			inverse(m_view) *
			//rotate(glm::radians(static_cast<float>(Input::deltaCursorPosition().x) / 15.f), vec3(0, 1, 0)) *
			rotate(glm::radians(static_cast<float>(Input::deltaCursorPosition().y) / 15.f), vec3(1, 0, 0)));

	}
	m_view = inverse(
		inverse(m_view) *
		translate(vec3(0.f, static_cast<float>(Input::getScrollPosition().y), 0.f)));

	if (Input::getKey(GLFW_KEY_F1) == GLFW_PRESS)
		m_shouldDrawGrid = !m_shouldDrawGrid;

	if (Input::getKey(GLFW_KEY_1, GLFW_MOD_SHIFT | GLFW_MOD_CONTROL) >= GLFW_PRESS)
		m_sun.transform().translate(vec3(0.f, 1.f * m_deltaTime, 0.f));

	if (Input::getKey(GLFW_KEY_2) >= GLFW_PRESS)
		m_earth.transform().translate(vec3(0.f, 1.f * m_deltaTime, 0.f));

	if (Input::getKey(GLFW_KEY_3) >= GLFW_PRESS)
		m_moon.transform().setPosition(vec3(0, 5, 0));
}

void MyApplication::update()
{
	m_sun.update(m_deltaTime);
	m_earth.update(m_deltaTime);
	m_moon.update(m_deltaTime);
}

void MyApplication::lateUpdate() { }

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

void MyApplication::drawSolarSystem()
{
	Gizmos::addSphere(
		m_sun.transform().getPosition(),
		m_sun.radius(), 25, 25,
		m_sun.colour(),
		&m_sun.transform().getWorldSpaceMatrix());

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

void MyApplication::drawGrid() const
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