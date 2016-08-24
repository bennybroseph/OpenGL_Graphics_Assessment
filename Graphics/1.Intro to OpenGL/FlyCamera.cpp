#include "FlyCamera.h"
#include "Input.h"


FlyCamera::FlyCamera()
{
	setLookAt(vec3(10, 10, 10), vec3(0), m_up);
	setPerspective(glm::pi<float>() * 0.25f, 16 / 9.f, 0.1f, 1000.f);
}

void FlyCamera::update(const float &deltaTime)
{
	if (Input::getMouseButton(GLFW_MOUSE_BUTTON_1) >= GLFW_PRESS)
	{
		m_worldTransform.rotate(static_cast<float>(Input::deltaCursorPosition().x) / 16.f, vec3(0.f, 1.f, 0.f));
		m_worldTransform.rotate(static_cast<float>(Input::deltaCursorPosition().y) / 9.f, vec3(1.f, 0.f, 0.f));
	}

	if (Input::getKey(GLFW_KEY_W) >= GLFW_PRESS)
		m_worldTransform.translate(vec3(0, 0, -m_speed * deltaTime));
	if (Input::getKey(GLFW_KEY_A) >= GLFW_PRESS)
		m_worldTransform.translate(vec3(-m_speed * deltaTime, 0, 0));
	if (Input::getKey(GLFW_KEY_S) >= GLFW_PRESS)
		m_worldTransform.translate(vec3(0, 0, m_speed * deltaTime));
	if (Input::getKey(GLFW_KEY_D) >= GLFW_PRESS)
		m_worldTransform.translate(vec3(m_speed * deltaTime, 0, 0));
}

FlyCamera::~FlyCamera() { }
