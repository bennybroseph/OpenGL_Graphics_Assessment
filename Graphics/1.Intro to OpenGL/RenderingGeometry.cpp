#include "RenderingGeometry.h"

#include "Input.h"
#include "FlyCamera.h"
#include "Gizmos.h"

int RenderingGeometry::startup()
{
	createWindow("Rendering Geometry", 1600, 900);

	Input::init();
	Shader::init();

	Gizmos::init();

	m_camera = make_unique<GameObject>();
	m_camera->setName("Main Camera");
	m_camera->addComponent<FlyCamera>();

	m_plane = make_unique<GameObject>();
	m_plane->transform()->setScale(3.f);
	m_plane->addComponent(Gizmos::Plane::createModel(32.f));

	auto model = m_plane->getComponent<Model>();
	model->setShader(Shader::time());
	model->setName("Animated Plane");

	return true;
}

void RenderingGeometry::shutdown()
{
	Input::quit();
	Shader::quit();

	Gizmos::quit();

	destroyWindow();
}

void RenderingGeometry::parseInput()
{
	// clear the screen for this frame
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// close the application if the window closes or we press escape
	if (glfwWindowShouldClose(m_window) || Input::getKey(GLFW_KEY_ESCAPE) >= GLFW_PRESS)
		m_isRunning = false;
}

void RenderingGeometry::update()
{
	m_camera->getComponent<FlyCamera>()->update(m_deltaTime);
}

void RenderingGeometry::lateUpdate()
{
	Input::lateUpdate();
}

void RenderingGeometry::draw()
{
	Transform::drawGizmos(mat4(1), 0);

	m_plane->draw();
	Gizmos::drawGrid(vec3(0.f), vec2(1.f, 1.f), vec2(5.f, 5.f));
}
