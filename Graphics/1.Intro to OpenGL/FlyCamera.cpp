#include "FlyCamera.h"
#include "Input.h"


FlyCamera::FlyCamera()
{
	setLookAt(vec3(0.f, 2.5f, 5.f), vec3(0), m_up);
	setPerspective(glm::pi<float>() * 0.25f, 16 / 9.f, 0.1f, 1000.f);

	if (s_mainCamera == nullptr)
		s_mainCamera = this;
}

void FlyCamera::update(const float &deltaTime)
{
	if (Input::getMouseButton(GLFW_MOUSE_BUTTON_1) >= GLFW_PRESS)
	{
		auto thetaYaw = glm::radians(-static_cast<float>(Input::deltaCursorPosition().x) / 16.f);
		auto yawMat = rotate(thetaYaw, vec3(0.f, 1.f, 0.f));

		auto thetaPitch = glm::radians(-static_cast<float>(Input::deltaCursorPosition().y) / 9.f);
		auto pitchMat = rotate(thetaPitch, vec3(1.f, 0.f, 0.f));

		// Somehow I need to combine the yaw and pitch but I've spent hours on this and I can't figure it out no 
		// matter how hard I try
		auto thetaRoll = glm::radians(thetaPitch) - glm::radians(thetaYaw);
		//printf("%f \n", glm::degrees(thetaRoll));
		auto rollMat = rotate(thetaRoll, vec3(0.f, 0.f, 1.f));

		m_worldTransform.setLocalSpaceMatrix(m_worldTransform.getLocalSpaceMatrix() * yawMat *  pitchMat);
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
