#include "ProceduralGeneration.h"

#include "Input.h"
#include "FlyCamera.h"
#include "Gizmos.h"

int ProceduralGeneration::startup()
{
	createWindow("Procedural Generation", 1600, 900);

	Input::init();
	Shader::init();

	Gizmos::init();

	m_camera = make_unique<GameObject>();
	m_camera->setName("Main Camera");
	m_camera->addComponent<FlyCamera>();

	m_plane = make_unique<GameObject>();
	m_plane->transform()->setScale(10.f);
	m_plane->addComponent(Gizmos::Plane::createModel(64.f));

	auto model = m_plane->getComponent<Model>();
	model->setShader(Shader::perlin());
	model->setName("Animated Plane");

	return true;
}

void ProceduralGeneration::shutdown()
{
	Input::quit();
	Shader::quit();

	Gizmos::quit();

	destroyWindow();
}

void ProceduralGeneration::parseInput()
{
	// clear the screen for this frame
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// close the application if the window closes or we press escape
	if (glfwWindowShouldClose(m_window) || Input::getKey(GLFW_KEY_ESCAPE) >= GLFW_PRESS)
		m_isRunning = false;
}

void ProceduralGeneration::update()
{
	m_camera->getComponent<FlyCamera>()->update(m_deltaTime);
}

void ProceduralGeneration::lateUpdate()
{
	Input::lateUpdate();
}

void ProceduralGeneration::draw()
{
	m_plane->draw();
	Gizmos::drawGrid(vec3(0.f), vec2(1.f, 1.f), vec2(5.f, 5.f));
}
