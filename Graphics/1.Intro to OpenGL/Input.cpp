#include "Input.h"

#include <imgui_impl_glfw_gl3.h>

#pragma region // Static Initializer

vectorPtrU<OnKeyCallback> Input::m_onKeyCallbacks = unique_ptr<vector<OnKeyCallback>>();
vectorPtrU<OnCursorPosCallback> Input::m_onCursorPosCallback = unique_ptr<vector<OnCursorPosCallback>>();
vectorPtrU<OnMouseButtonCallback> Input::m_onMouseButtonCallbacks = unique_ptr<vector<OnMouseButtonCallback>>();
vectorPtrU<OnScrollCallback> Input::m_onScrollCallback = unique_ptr<vector<OnScrollCallback>>();
vectorPtrU<OnCursorEnterCallback> Input::m_onCursorEnterCallback = unique_ptr<vector<OnCursorEnterCallback>>();

unique_ptr<Input> Input::self = unique_ptr<Input>();

#pragma endregion

void Input::init()
{
	m_onKeyCallbacks.reset(new vector<OnKeyCallback>);
	m_onCursorPosCallback.reset(new vector<OnCursorPosCallback>);
	m_onMouseButtonCallbacks.reset(new vector<OnMouseButtonCallback>);
	m_onScrollCallback.reset(new vector<OnScrollCallback>);
	m_onCursorEnterCallback.reset(new vector<OnCursorEnterCallback>);

	auto window = glfwGetCurrentContext();

	glfwSetKeyCallback(window, onKey);
	glfwSetCursorPosCallback(window, onCursorPos);
	glfwSetMouseButtonCallback(window, onMouseButton);
	glfwSetScrollCallback(window, onScroll);
	glfwSetCursorEnterCallback(window, onCursorEnter);

	new Input;
}

GLint Input::getKey(GLint key)
{
	if (ImGui::GetIO().WantCaptureKeyboard || !self->m_keyStates.count(key))
		return GLFW_RELEASED;

	return self->m_keyStates[key].state;
}
GLint Input::getKey(GLint key, GLint mods)
{
	if (ImGui::GetIO().WantCaptureKeyboard || !self->m_keyStates.count(key))
		return GLFW_RELEASED;

	if (self->m_keyStates[key].mods == mods)
		return  self->m_keyStates[key].state;

	return GLFW_RELEASED;
}

const glm::dvec2& Input::getCursorPosition()
{
	return self->m_cursorPos;
}
glm::dvec2 Input::deltaCursorPosition()
{
	if (ImGui::GetIO().WantCaptureMouse)
		return glm::dvec2();

	return self->m_cursorPos - self->m_prevCursorPos;
}

GLint Input::getMouseButton(GLint button)
{
	if (ImGui::GetIO().WantCaptureMouse || !self->m_mouseButtonStates.count(button))
		return GLFW_RELEASED;

	return self->m_mouseButtonStates[button].state;
}
GLint Input::getMouseButton(GLint button, GLint mods)
{
	if (ImGui::GetIO().WantCaptureMouse || !self->m_mouseButtonStates.count(button))
		return GLFW_RELEASED;

	if (self->m_mouseButtonStates[button].mods == mods)
		return self->m_mouseButtonStates[button].state;

	return GLFW_RELEASED;
}

glm::dvec2 Input::getScrollPosition()
{
	if (ImGui::GetIO().WantCaptureMouse)
		return glm::dvec2();

	return self->m_scrollPos;
}

#pragma region // Add Callback Functions

void Input::addOnKeyCallback(const OnKeyCallback &delegate)
{
	m_onKeyCallbacks->push_back(delegate);
}
void Input::addOnCursorPosCallback(const OnCursorPosCallback &delegate)
{
	m_onCursorPosCallback->push_back(delegate);
}
void Input::addOnMouseButtonCallback(const OnMouseButtonCallback &delegate)
{
	m_onMouseButtonCallbacks->push_back(delegate);
}
void Input::addOnScrollCallback(const OnScrollCallback &delegate)
{
	m_onScrollCallback->push_back(delegate);
}
void Input::addOnCursorEnterCallback(const OnCursorEnterCallback &delegate)
{
	m_onCursorEnterCallback->push_back(delegate);
}

#pragma endregion

void Input::lateUpdate()
{
	for (auto &pair : self->m_keyStates)
	{
		if (pair.second.state == GLFW_PRESS)
			pair.second.state = GLFW_REPEAT;
		if (pair.second.state == GLFW_RELEASE)
			pair.second.state = GLFW_RELEASED;
	}

	self->m_prevCursorPos = self->m_cursorPos;

	self->m_scrollPos.x = 0;
	self->m_scrollPos.y = 0;
}

void Input::onKey(GLFWwindow *window, GLint key, GLint scanecode, GLint action, GLint mods)
{
	switch (action)
	{
	case GLFW_PRESS:
	case GLFW_RELEASE:
		self->m_keyStates[key].state = action;
		self->m_keyStates[key].mods = mods;
		break;

	default:
		self->m_keyStates[key].mods = mods;
		break;
	}

	if (ImGui::GetIO().WantCaptureKeyboard)
		return;

	for (const auto &delegate : *m_onKeyCallbacks)
		delegate(window, key, scanecode, action, mods);
}

void Input::onCursorPos(GLFWwindow *window, GLdouble x, GLdouble y)
{
	self->m_cursorPos.x = x;
	self->m_cursorPos.y = y;

	for (const auto &delegate : *m_onCursorPosCallback)
		delegate(window, x, y);
}

void Input::onMouseButton(GLFWwindow* window, GLint button, GLint action, GLint mods)
{
	switch (action)
	{
	case GLFW_PRESS:
	case GLFW_RELEASE:
		self->m_mouseButtonStates[button].state = action;
		self->m_mouseButtonStates[button].mods = mods;
		break;

	default:
		self->m_mouseButtonStates[button].mods = mods;
		break;
	}

	if (ImGui::GetIO().WantCaptureMouse)
		return;

	for (const auto &delegate : *m_onMouseButtonCallbacks)
		delegate(window, button, action, mods);
}

void Input::onScroll(GLFWwindow* window, GLdouble x, GLdouble y)
{
	self->m_scrollPos.x = x;
	self->m_scrollPos.y = y;

	if (ImGui::GetIO().WantCaptureMouse)
		return;

	for (const auto &delegate : *m_onScrollCallback)
		delegate(window, x, y);
}

void Input::onCursorEnter(GLFWwindow* window, GLint state)
{
	for (const auto &delegate : *m_onCursorEnterCallback)
		delegate(window, state);
}

void Input::quit()
{
	m_onKeyCallbacks.reset();
	m_onCursorPosCallback.reset();
	m_onMouseButtonCallbacks.reset();
	m_onScrollCallback.reset();
	m_onCursorEnterCallback.reset();

	self.reset();
}

Input::Input()
{
	self.reset(this);
}