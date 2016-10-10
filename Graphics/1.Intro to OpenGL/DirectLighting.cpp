#include "DirectLighting.h"

#include "Input.h"
#include "FlyCamera.h"
#include "Gizmos.h"
#include "DirectionalLight.h"

int DirectLighting::startup()
{
	createWindow("Direct Lighting", 1600, 900);

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

	m_sphere = make_unique<GameObject>();
	m_sphere->transform()->setScale(3.f);
	m_sphere->addComponent(Gizmos::Sphere::create());

	auto model = m_sphere->getComponent<Model>();
	model->setShader(Shader::phong());
	model->setName("Lit Sphere");

	return true;
}

void DirectLighting::shutdown()
{
	Input::quit();
	Shader::quit();

	Gizmos::quit();

	destroyWindow();
}

void DirectLighting::parseInput()
{
	// clear the screen for this frame
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// close the application if the window closes or we press escape
	if (glfwWindowShouldClose(m_window) || Input::getKey(GLFW_KEY_ESCAPE) >= GLFW_PRESS)
		m_isRunning = false;
}

void DirectLighting::update()
{
	m_light->transform()->rotate(40.f * m_deltaTime, vec3(0.f, 1.f, -1.f));

	m_camera->getComponent<FlyCamera>()->update(m_deltaTime);
}

void DirectLighting::lateUpdate()
{
	Input::lateUpdate();
}

void DirectLighting::draw()
{
	m_light->draw();

	m_sphere->draw();
	Gizmos::drawGrid(vec3(0.f), vec2(1.f, 1.f), vec2(5.f, 5.f));

	m_light->drawGizmos();
}
