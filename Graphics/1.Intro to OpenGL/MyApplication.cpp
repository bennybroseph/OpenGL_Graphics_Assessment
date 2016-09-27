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

using std::fstream;

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

	// setup some camera stuff
	m_camera = make_unique<FlyCamera>();

	m_light.reset(new DirectionalLight);
	m_light->m_transform->setPosition(vec3(0.f, 5.f, 0.f));
	m_light->m_transform->rotate(45.f, vec3(1.f, 0.f, 0.f));
	*m_light->m_diffuse = vec3(1.f, 1.f, 1.f);

	m_sun = make_unique<GameObject>();
	m_sun->setName("Sun");
	m_earth = make_unique<GameObject>();
	m_earth->transform()->setParent(m_sun->transform(), false);
	m_moon = make_unique<GameObject>();
	m_moon->transform()->setParent(m_earth->transform(), false);

	m_sun->addComponent(Gizmos::Plane::create());
	auto model = m_sun->getComponent<Model>();
	model->setShader(Shader::texture());
	//model->addTexture("data/textures/crate.png", FilteringType::Nearest);
	model->setDiffuseTexture("data/textures/four_diffuse.tga", FilteringType::Linear);
	model->setNormalTexture("data/textures/four_normal.tga", FilteringType::Linear);
	model->setSpecularTexture("data/textures/four_specular.tga", FilteringType::Linear);

	/*auto newSphere = Gizmos::Sphere::create();
	newSphere->setShader(Shader::texture());
	newSphere->transform()->setLocalPosition(vec3(0.f, 1.5f, 0.f));
	newSphere->addTexture("data/textures/planets/earth_diffuse.jpg", FilteringType::Linear);
	m_shapes->push_back(move(newSphere));*/

	auto newPlane = make_unique<GameObject>();

	auto plane = Gizmos::Sphere::create();
	plane->setShader(Shader::phong());
	plane->setMaterialColour(vec4(1));

	newPlane->addComponent(move(plane));

	newPlane->transform()->setLocalPosition(vec3(2.f, 0.f, 0.f));
	newPlane->transform()->setParent(m_sun->transform());
	m_shapes->push_back(move(newPlane));

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
	Shader::quit();
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
}

void MyApplication::drawGui()
{
	m_light->draw();

	m_sun->transform()->draw();
	m_earth->transform()->draw();
	m_moon->transform()->draw();

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
		m_sun->drawGui();
		m_earth->drawGui();
		m_moon->drawGui();
	}
	ImGui::End();

	ImGui::Render();
}

MyApplication::~MyApplication() { }