#include "FlyCamera.h"
#include "Input.h"


FlyCamera::FlyCamera()
{
	setLookAt(vec3(0.f, 5.f, 10.f), vec3(0), *m_up);
	setPerspective(glm::pi<float>() * 0.25f, 16 / 9.f, 0.1f, 75.f);

	if (s_mainCamera == nullptr)
		s_mainCamera = this;
}

void FlyCamera::update(const float &deltaTime) const
{
	if (Input::getMouseButton(GLFW_MOUSE_BUTTON_1) >= GLFW_PRESS)
	{
		auto thetaYaw = -static_cast<float>(Input::deltaCursorPosition().x) / 16.f;
		auto thetaPitch = -static_cast<float>(Input::deltaCursorPosition().y) / 9.f;

		m_rotation->x += thetaPitch;
		m_rotation->y += thetaYaw;

		m_worldTransform->setLocalEulerAngle(*m_rotation);
	}

	if (Input::getKey(GLFW_KEY_W) >= GLFW_PRESS)
		m_worldTransform->translate(vec3(0, 0, -*m_speed * deltaTime));
	if (Input::getKey(GLFW_KEY_A) >= GLFW_PRESS)
		m_worldTransform->translate(vec3(-*m_speed * deltaTime, 0, 0));
	if (Input::getKey(GLFW_KEY_S) >= GLFW_PRESS)
		m_worldTransform->translate(vec3(0, 0, *m_speed * deltaTime));
	if (Input::getKey(GLFW_KEY_D) >= GLFW_PRESS)
		m_worldTransform->translate(vec3(*m_speed * deltaTime, 0, 0));
}

FlyCamera::~FlyCamera()
{
	Camera::~Camera();
}
