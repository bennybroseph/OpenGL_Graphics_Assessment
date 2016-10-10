#include "CamerasAndProjections.h"

#include "Input.h"
#include "FlyCamera.h"
#include "Gizmos.h"

int CamerasAndProjections::startup()
{
	createWindow("Cameras and Projections", 1600, 900);

	Input::init();
	Shader::init();

	Gizmos::init();

	m_camera = make_unique<GameObject>();
	m_camera->setName("Main Camera");
	m_camera->addComponent<FlyCamera>();

	return true;
}

void CamerasAndProjections::shutdown()
{
	Input::quit();
	Shader::quit();

	Gizmos::quit();

	destroyWindow();
}

void CamerasAndProjections::parseInput()
{
	// clear the screen for this frame
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// close the application if the window closes or we press escape
	if (glfwWindowShouldClose(m_window) || Input::getKey(GLFW_KEY_ESCAPE) >= GLFW_PRESS)
		m_isRunning = false;
}

void CamerasAndProjections::update()
{
	m_camera->getComponent<FlyCamera>()->update(m_deltaTime);
}

void CamerasAndProjections::lateUpdate()
{
	Input::lateUpdate();
}

void CamerasAndProjections::draw()
{
	Gizmos::drawGrid(vec3(0.f), vec2(1.f, 1.f), vec2(5.f, 5.f));
	Transform::drawGizmos(mat4(1));
}
