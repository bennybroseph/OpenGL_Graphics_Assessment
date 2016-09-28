#include "MyApplication.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <imgui_impl_glfw_gl3.h>

#include "Math.h"
#include "Input.h"
#include "Gizmos.h"
#include "Shader.h"
#include "FlyCamera.h"
#include "DirectionalLight.h"
#include "Inspector.h"

using std::fstream;

using Editor::Inspector;

const static int GRID_SIZE = 10;
const static int GRID_SEPARATOR = 5;

int MyApplication::startup()
{
	createWindow("Intro to OpenGL", static_cast<int>(m_screenSize.x), static_cast<int>(m_screenSize.y));

	//Initialize ImGUI
	ImGui_ImplGlfwGL3_Init(m_window, true);

	auto &io = ImGui::GetIO();

	io.DisplaySize.x = m_screenSize.x;
	io.DisplaySize.y = m_screenSize.y;

	Input::init();
	Shader::init();
	Gizmos::init();
	Inspector::init();

	// setup some camera stuff
	m_camera = make_unique<GameObject>();
	m_camera->setName("Main Camera");
	m_camera->addComponent<FlyCamera>();

	m_light = make_unique<GameObject>();
	m_light->setName("Directional Light");
	m_light->addComponent<DirectionalLight>();
	m_light->transform()->setPosition(vec3(0.f, 5.f, 0.f));
	m_light->transform()->rotate(45.f, vec3(1.f, 0.f, 0.f));

	m_sun = make_unique<GameObject>();
	m_sun->setName("Sun");
	m_sun->addComponent(Gizmos::Sphere::create());

	auto model = m_sun->getComponent<Model>();
	model->setMaterialColour(vec4(1, 1, 0, 1));

	m_earth = make_unique<GameObject>();
	m_earth->setName("Earth");
	m_earth->transform()->setLocalPosition(vec3(2.f, 0.f, 0.f));
	m_earth->transform()->setParent(m_sun->transform(), false);
	m_earth->addComponent(Gizmos::Sphere::create());

	model = m_earth->getComponent<Model>();
	model->setMaterialColour(vec4(0, 1, 0, 1));

	m_moon = make_unique<GameObject>();
	m_moon->setName("Moon");
	m_moon->transform()->setLocalPosition(vec3(1.5f, 0.f, 0.f));
	m_moon->transform()->setParent(m_earth->transform(), false);
	m_moon->addComponent(Gizmos::Sphere::create());

	model = m_moon->getComponent<Model>();
	model->setMaterialColour(vec4(1, 1, 1, 1));

	auto newPlane = make_unique<GameObject>();
	newPlane->setName("Test Plane");

	newPlane->addComponent(Gizmos::Plane::create());
	model = newPlane->getComponent<Model>();
	model->setShader(Shader::texture());
	//model->addTexture("data/textures/crate.png", FilteringType::Nearest);
	model->setDiffuseTexture("data/textures/four_diffuse.tga", FilteringType::Linear);
	model->setNormalTexture("data/textures/four_normal.tga", FilteringType::Linear);
	model->setSpecularTexture("data/textures/four_specular.tga", FilteringType::Linear);
	m_shapes->push_back(move(newPlane));

	auto newSphere = make_unique<GameObject>();
	newSphere->setName("Test Sphere");

	newSphere->addComponent(Gizmos::Sphere::create());
	model = newSphere->getComponent<Model>();
	model->setShader(Shader::phong());
	model->setMaterialColour(vec4(1));

	newSphere->transform()->setLocalPosition(vec3(-2.f, 0.f, 0.f));
	newSphere->transform()->setParent(m_sun->transform());
	m_shapes->push_back(move(newSphere));

	return true;
}

void MyApplication::shutdown()
{
	Input::quit();
	Shader::quit();
	Gizmos::quit();
	Inspector::quit();

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
		m_sun->transform()->translate(vec3(0.f, 1.f * m_deltaTime, 0.f));

	if (Input::getKey(GLFW_KEY_2) >= GLFW_PRESS)
		m_earth->transform()->setPosition(
			vec3(
				m_earth->transform()->getPosition().x,
				m_earth->transform()->getPosition().y + 1.f * m_deltaTime,
				m_earth->transform()->getPosition().z));

	if (Input::getKey(GLFW_KEY_3) >= GLFW_PRESS)
	{
		m_moon->transform()->setPosition(vec3(5.f, 2.5f, -5.5f));
		m_moon->transform()->setEulerAngle(vec3(90.f, 0, 0));
		m_moon->transform()->setScale(5.f);
	}

	if (Input::getKey(GLFW_KEY_PERIOD, GLFW_MOD_SHIFT) >= GLFW_PRESS)
		m_sun->transform()->scale(vec3(1.f + m_deltaTime, 1.f + m_deltaTime, 1.f + m_deltaTime));

	if (Input::getKey(GLFW_KEY_COMMA, GLFW_MOD_SHIFT) >= GLFW_PRESS)
		m_sun->transform()->scale(vec3(1.f - m_deltaTime, 1.f - m_deltaTime, 1.f - m_deltaTime));
}

void MyApplication::update()
{
	//m_light->m_transform.rotate(15.f * m_deltaTime, vec3(0.f, 1.f, -1.f));

	m_camera->getComponent<FlyCamera>()->update(m_deltaTime);
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
		Gizmos::drawGrid(vec3(0.f), vec2(1.f, 1.f), vec2(5.f, 5.f));

	for (auto &shape : *m_shapes)
		shape->draw();

	drawSolarSystem();

	if (m_shouldDrawGui)
		drawGui();
}

void MyApplication::drawSolarSystem() const
{
	m_sun->draw();
	m_earth->draw();
	m_moon->draw();

	m_light->draw();
}

void MyApplication::drawGui()
{
	Inspector::drawGui();

	ImGui::Begin("Test Values");
	{
		if (ImGui::ColorEdit3("Clear Colour", value_ptr(m_clearColour)))
			glClearColor(m_clearColour.r, m_clearColour.g, m_clearColour.b, m_clearColour.a);

		ImGui::Checkbox("Draw Grid", &m_shouldDrawGrid);
	}
	ImGui::End();

	ImGui::Begin("Debug");
	{

	}
	ImGui::End();

	ImGui::Render();
}

MyApplication::~MyApplication() { }