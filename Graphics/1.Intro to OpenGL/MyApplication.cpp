#include "MyApplication.h"

#include <fstream>
#include <imgui_impl_glfw_gl3.h>

#include "Math.h"
#include "Input.h"
#include "Gizmos.h"
#include "Shader.h"
#include "FlyCamera.h"
#include "DirectionalLight.h"

using std::fstream;

const static int GRID_SIZE = 10;
const static int GRID_SEPARATOR = 5;

int MyApplication::startup()
{
	createWindow("Intro to OpenGL", m_screenSize.x, m_screenSize.y);

	//Initialize ImGUI
	ImGui_ImplGlfwGL3_Init(m_window, true);

	auto &io = ImGui::GetIO();
	io.DisplaySize.x = m_screenSize.x;
	io.DisplaySize.y = m_screenSize.y;

	Input::init();
	Shader::init();
	Gizmos::init();

	// setup some camera stuff
	m_camera = new FlyCamera();

	m_light = new DirectionalLight();
	m_light->m_transform->setPosition(vec3(0.f, 5.f, 0.f));
	m_light->m_transform->rotate(45.f, vec3(1.f, 0.f, 0.f));
	*m_light->m_diffuse = vec3(1.f, 1.f, 1.f);

	m_earth->transform().setParent(&m_sun->transform(), false);
	m_moon->transform().setParent(&m_earth->transform(), false);

	/*auto newSphere = new Sphere();
	newSphere->shader() = Shader::phongShader();
	newSphere->transform().setLocalPosition(vec3(0.f, 1.5f, 0.f));
	m_shapes.push_back(newSphere);*/

	/*auto newPlane = new Plane();
	newPlane->shader() = Shader::phongShader();
	newPlane->transform().scale(vec3(5.f, 1.f, 5.f));
	newPlane->transform().setParent(&m_sun.transform());
	m_shapes.push_back(newPlane);*/

	/*auto newCube = new Cube();
	newCube->transform().setParent(&m_sun.transform());
	newCube->transform().setLocalPosition(vec3(1.f, 0.5f, 0.f));
	newCube->transform().setLocalEulerAngle(vec3(45.f, 0.f, 45.f));

	m_shapes.push_back(newCube);*/

	return true;
}

void MyApplication::shutdown()
{
	Input::quit();
	Gizmos::quit();
	ImGui_ImplGlfwGL3_Shutdown();

	destroyWindow();
}

void MyApplication::parseInput()
{
	ImGui_ImplGlfwGL3_NewFrame();

	// close the application if the window closes or we press escape
	if (glfwWindowShouldClose(m_window) || Input::getKey(GLFW_KEY_ESCAPE) >= GLFW_PRESS)
	{
		m_isRunning = false;
		return;
	}

	if (Input::getKey(GLFW_KEY_F1) == GLFW_PRESS)
		m_shouldDrawGui = !m_shouldDrawGui;

	if (Input::getKey(GLFW_KEY_1) >= GLFW_PRESS)
		m_sun->transform().translate(vec3(0.f, 1.f * m_deltaTime, 0.f));

	if (Input::getKey(GLFW_KEY_2) >= GLFW_PRESS)
		m_earth->transform().setPosition(
			vec3(
				m_earth->transform().getPosition().x,
				m_earth->transform().getPosition().y + 1.f * m_deltaTime,
				m_earth->transform().getPosition().z));

	if (Input::getKey(GLFW_KEY_3) >= GLFW_PRESS)
	{
		m_moon->transform().setPosition(vec3(5.f, 2.5f, -5.5f));
		m_moon->transform().setEulerAngle(vec3(90.f, 0, 0));
		m_moon->transform().setScale(5.f);
	}

	if (Input::getKey(GLFW_KEY_PERIOD, GLFW_MOD_SHIFT) >= GLFW_PRESS)
		m_sun->transform().scale(vec3(1.f + m_deltaTime, 1.f + m_deltaTime, 1.f + m_deltaTime));

	if (Input::getKey(GLFW_KEY_COMMA, GLFW_MOD_SHIFT) >= GLFW_PRESS)
		m_sun->transform().scale(vec3(1.f - m_deltaTime, 1.f - m_deltaTime, 1.f - m_deltaTime));
}

void MyApplication::update()
{
	//m_light->m_transform.rotate(15.f * m_deltaTime, vec3(0.f, 1.f, -1.f));

	m_sun->update(m_deltaTime);
	m_earth->update(m_deltaTime);
	m_moon->update(m_deltaTime);

	m_camera->update(m_deltaTime);
}

void MyApplication::lateUpdate()
{
	Input::lateUpdate();
}

void MyApplication::draw()
{
	// clear the screen for this frame
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (m_shouldDrawGrid)
		Gizmos::drawGrid(vec3(0), vec2(1.f, 1.f), vec2(5.f, 5.f), vec4(1.f));

	for (auto shape : *m_shapes)
		shape->draw();

	drawSolarSystem();

	m_light->draw();

	if (m_shouldDrawGui)
		drawGui();
}

void MyApplication::drawSolarSystem()
{
	Gizmos::drawSphere(m_sun->transform().getWorldSpaceMatrix(), m_sun->colour());
	Gizmos::drawSphere(m_earth->transform().getWorldSpaceMatrix(), m_earth->colour());
	Gizmos::drawSphere(m_moon->transform().getWorldSpaceMatrix(), m_moon->colour());

	m_sun->transform().draw();
	m_earth->transform().draw();
	m_moon->transform().draw();
}

void MyApplication::drawGui()
{
	ImGui::Begin("Test Values");
	{
		if (ImGui::ColorEdit3("Clear Colour", value_ptr(m_clearColour)))
			glClearColor(m_clearColour.r, m_clearColour.g, m_clearColour.b, m_clearColour.a);
		ImGui::Checkbox("Draw Grid", &m_shouldDrawGrid);
	}
	ImGui::End();

	m_light->drawGui();

	ImGui::Begin("Debug");
	{
		m_sun->transform().drawGui();
		m_earth->transform().drawGui();
		m_moon->transform().drawGui();
	}
	ImGui::End();

	ImGui::Render();
}

MyApplication::~MyApplication()
{
	for (auto shape : *m_shapes)
		delete shape;
}
