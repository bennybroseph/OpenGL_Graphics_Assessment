#include "Texturing.h"

#include "Input.h"
#include "FlyCamera.h"
#include "Gizmos.h"
#include "DirectionalLight.h"

int Texturing::startup()
{
	createWindow("Texturing", 1600, 900);

	Input::init();
	Shader::init();

	Gizmos::init();

	m_camera = make_unique<GameObject>();
	m_camera->setName("Main Camera");
	m_camera->addComponent<FlyCamera>();

	m_light = make_unique<GameObject>();
	m_light->setName("Directional Light");
	m_light->addComponent<DirectionalLight>();
	m_light->transform()->setPosition(vec3(0.f, 5.f, 0.f));
	m_light->transform()->rotate(45.f, vec3(1.f, 0.f, 0.f));

	m_plane = make_unique<GameObject>();
	m_plane->transform()->setScale(3.f);
	m_plane->addComponent(Gizmos::Plane::createModel());
	m_plane->transform()->translate(vec3(0.f, 0.01f, 0.f));

	auto model = m_plane->getComponent<Model>();
	model->setShader(Shader::texture());
	model->setName("Animated Plane");
	model->setDiffuseTexture("data/textures/four_diffuse.tga", FilteringType::Linear);
	model->setNormalTexture("data/textures/four_normal.tga", FilteringType::Linear);
	model->setSpecularTexture("data/textures/four_specular.tga", FilteringType::Linear);

	return true;
}

void Texturing::shutdown()
{
	Input::quit();
	Shader::quit();

	Gizmos::quit();

	destroyWindow();
}

void Texturing::parseInput()
{
	// clear the screen for this frame
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// close the application if the window closes or we press escape
	if (glfwWindowShouldClose(m_window) || Input::getKey(GLFW_KEY_ESCAPE) >= GLFW_PRESS)
		m_isRunning = false;
}

void Texturing::update()
{
	m_light->transform()->rotate(40.f * m_deltaTime, vec3(0.f, 1.f, -1.f));

	m_camera->getComponent<FlyCamera>()->update(m_deltaTime);
}

void Texturing::lateUpdate()
{
	Input::lateUpdate();
}

void Texturing::draw()
{
	m_light->draw();

	m_plane->draw();
	Gizmos::drawGrid(vec3(0.f), vec2(1.f, 1.f), vec2(5.f, 5.f));

	m_light->drawGizmos();
}
