#include "FlyCamera.h"

#include "Input.h"
#include "GameObject.h"

FlyCamera::FlyCamera() : Camera()
{
	setName("Fly Camera");
}

void FlyCamera::init()
{
	setLookAt(vec3(0.f, 5.f, 10.f), vec3(0), *m_up);
	setPerspective(PI * 0.25f, 16 / 9.f, 0.1f, 75.f);
}

void FlyCamera::update(const float &deltaTime) const
{
	if (Input::getMouseButton(GLFW_MOUSE_BUTTON_1) >= GLFW_PRESS)
	{
		auto thetaYaw = -static_cast<float>(Input::deltaCursorPosition().x) / 16.f;
		auto thetaPitch = -static_cast<float>(Input::deltaCursorPosition().y) / 9.f;

		m_rotation->x += thetaPitch;
		m_rotation->y += thetaYaw;

		m_gameObject->transform()->setLocalEulerAngle(*m_rotation);
	}

	if (Input::getKey(GLFW_KEY_W) >= GLFW_PRESS)
		m_gameObject->transform()->translate(vec3(0, 0, -m_speed * deltaTime));
	if (Input::getKey(GLFW_KEY_A) >= GLFW_PRESS)
		m_gameObject->transform()->translate(vec3(-m_speed * deltaTime, 0, 0));
	if (Input::getKey(GLFW_KEY_S) >= GLFW_PRESS)
		m_gameObject->transform()->translate(vec3(0, 0, m_speed * deltaTime));
	if (Input::getKey(GLFW_KEY_D) >= GLFW_PRESS)
		m_gameObject->transform()->translate(vec3(m_speed * deltaTime, 0, 0));
}

FlyCamera::~FlyCamera() { }
