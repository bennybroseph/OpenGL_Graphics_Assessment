#include "Input.h"

#include <imgui_impl_glfw_gl3.h>

#pragma region // Static Initializer

vector<OnKeyCallback> *const Input::m_onKeyCallbacks = new vector<OnKeyCallback>();
const Input::KeyStateMapPtr Input::m_keyStates = make_unique<KeyStateMap>();

vector<OnCursorPosCallback> *const Input::m_onCursorPosCallback = new vector<OnCursorPosCallback>();

Input::Position *const Input::m_cursorPos = new Position();
Input::Position *const Input::m_prevCursorPos = new Position();

vector<OnMouseButtonCallback> *const Input::m_onMouseButtonCallbacks = new vector<OnMouseButtonCallback>();

const Input::KeyStateMapPtr Input::m_mouseButtonStates = make_unique<KeyStateMap>();

vector<OnScrollCallback> *const Input::m_onScrollCallback = new vector<OnScrollCallback>();

Input::Position *const Input::m_scrollPos = new Position();
Input::Position *const Input::m_prevScrollPos = new Position();

vector<OnCursorEnterCallback> *const Input::m_onCursorEnterCallback = new vector<OnCursorEnterCallback >();

#pragma endregion

void Input::init()
{
	auto window = glfwGetCurrentContext();

	glfwSetKeyCallback(window, onKey);
	glfwSetCursorPosCallback(window, onCursorPos);
	glfwSetMouseButtonCallback(window, onMouseButton);
	glfwSetScrollCallback(window, onScroll);
	glfwSetCursorEnterCallback(window, onCursorEnter);
}

int Input::getKey(const int &key)
{
	if (ImGui::GetIO().WantCaptureKeyboard || !m_keyStates->count(key))
		return GLFW_RELEASED;

	return (*m_keyStates)[key]->state;
}
int Input::getKey(const int &key, const int &mods)
{
	if (ImGui::GetIO().WantCaptureKeyboard || !m_keyStates->count(key))
		return GLFW_RELEASED;

	if ((*m_keyStates)[key]->mods == mods)
		return  (*m_keyStates)[key]->state;

	return GLFW_RELEASED;
}

const Input::Position& Input::getCursorPosition()
{
	return *m_cursorPos;
}
Input::Position Input::deltaCursorPosition()
{
	if (ImGui::GetIO().WantCaptureMouse)
		return Position();

	return *m_cursorPos - *m_prevCursorPos;
}

int Input::getMouseButton(const int &button)
{
	if (ImGui::GetIO().WantCaptureMouse || !m_mouseButtonStates->count(button))
		return GLFW_RELEASED;

	return (*m_mouseButtonStates)[button]->state;
}
int Input::getMouseButton(const int &button, const int &mods)
{
	if (ImGui::GetIO().WantCaptureMouse || !m_mouseButtonStates->count(button))
		return GLFW_RELEASED;

	if ((*m_mouseButtonStates)[button]->mods == mods)
		return (*m_mouseButtonStates)[button]->state;

	return GLFW_RELEASED;
}

Input::Position Input::getScrollPosition()
{
	if (ImGui::GetIO().WantCaptureMouse)
		return Position();

	return *m_scrollPos;
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
	for (auto &pair : *m_keyStates)
	{
		if (pair.second->state == GLFW_PRESS)
			pair.second->state = GLFW_REPEAT;
		if (pair.second->state == GLFW_RELEASE)
			pair.second->state = GLFW_RELEASED;
	}

	*m_prevCursorPos = *m_cursorPos;

	m_scrollPos->x = 0;
	m_scrollPos->y = 0;
}

void Input::onKey(GLFWwindow *window, int key, int scanecode, int action, int mods)
{
	auto it = m_keyStates->lower_bound(key);
	if (it == end(*m_keyStates) || m_keyStates->key_comp()(key, it->first))
		m_keyStates->insert(it, make_pair(key, make_unique<KeyState>(KeyState())));

	switch (action)
	{
	case GLFW_PRESS:
	case GLFW_RELEASE:
		(*m_keyStates)[key]->state = action;
		(*m_keyStates)[key]->mods = mods;
		break;

	default:
		(*m_keyStates)[key]->mods = mods;
		break;
	}

	if (ImGui::GetIO().WantCaptureKeyboard)
		return;

	for (const auto delegate : *m_onKeyCallbacks)
		delegate(key, scanecode, action, mods);
}

void Input::onCursorPos(GLFWwindow *window, double x, double y)
{
	m_cursorPos->x = x;
	m_cursorPos->y = y;

	for (const auto &delegate : *m_onCursorPosCallback)
		delegate(x, y);
}

void Input::onMouseButton(GLFWwindow* window, int button, int action, int mods)
{
	auto it = m_mouseButtonStates->lower_bound(button);
	if (it == end(*m_mouseButtonStates) || m_mouseButtonStates->key_comp()(button, it->first))
		m_mouseButtonStates->insert(it, make_pair(button, make_unique<KeyState>(KeyState())));

	switch (action)
	{
	case GLFW_PRESS:
	case GLFW_RELEASE:
		(*m_mouseButtonStates)[button]->state = action;
		(*m_mouseButtonStates)[button]->mods = mods;
		break;

	default:
		(*m_mouseButtonStates)[button]->mods = mods;
		break;
	}

	if (ImGui::GetIO().WantCaptureMouse)
		return;

	for (const auto &delegate : *m_onMouseButtonCallbacks)
		delegate(button, action, mods);
}

void Input::onScroll(GLFWwindow* window, double x, double y)
{
	m_scrollPos->x = x;
	m_scrollPos->y = y;

	if (ImGui::GetIO().WantCaptureMouse)
		return;

	for (const auto &delegate : *m_onScrollCallback)
		delegate(x, y);
}

void Input::onCursorEnter(GLFWwindow* window, int state)
{
	for (const auto &delegate : *m_onCursorEnterCallback)
		delegate(state);
}

void Input::quit() { }

