#include "Input.h"

#pragma region //{ Static Initializer
vector<OnKeyCallback> Input::m_onKeyCallbacks = vector<OnKeyCallback>();
map<int, Input::KeyState> Input::m_keyStates = map<int, KeyState>();

vector<OnCursorPosCallback> Input::m_onCursorPosCallback = vector<OnCursorPosCallback>();
Input::Position Input::m_cursorPos;
Input::Position Input::m_prevCursorPos;

vector<OnMouseButtonCallback> Input::m_onMouseButtonCallbacks = vector<OnMouseButtonCallback>();
map<int, Input::KeyState> Input::m_mouseButtonStates = map<int, KeyState>();

vector<OnScrollCallback> Input::m_onScrollCallback = vector<OnScrollCallback>();
Input::Position Input::m_scrollPos;
Input::Position Input::m_prevScrollPos;

vector<OnCursorEnterCallback> Input::m_onCursorEnterCallback = vector<OnCursorEnterCallback>();
#pragma endregion //}

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
	return m_keyStates[key].state;
}
int Input::getKey(const int &key, const int &mods)
{
	if (m_keyStates[key].mods == mods)
		return  m_keyStates[key].state;

	return GLFW_RELEASED;
}

const Input::Position& Input::getCursorPosition()
{
	return m_cursorPos;
}
Input::Position Input::deltaCursorPosition()
{
	return m_cursorPos - m_prevCursorPos;
}

int Input::getMouseButton(const int &button)
{
	return m_mouseButtonStates[button].state;
}
int Input::getMouseButton(const int &button, const int &mods)
{
	if (m_mouseButtonStates[button].mods == mods)
		return m_mouseButtonStates[button].state;

	return GLFW_RELEASED;
}

const Input::Position& Input::getScrollPosition()
{
	return m_scrollPos;
}
Input::Position Input::deltaScrollPosition()
{
	return m_scrollPos - m_prevScrollPos;
}
#pragma region //{ Add Callback Functions
void Input::addOnKeyCallback(const OnKeyCallback &delegate)
{
	m_onKeyCallbacks.push_back(delegate);
}
void Input::addOnCursorPosCallback(const OnCursorPosCallback &delegate)
{
	m_onCursorPosCallback.push_back(delegate);
}
void Input::onMouseButtonCallback(const OnMouseButtonCallback &delegate)
{
	m_onMouseButtonCallbacks.push_back(delegate);
}
void Input::onScrollCallback(const OnScrollCallback &delegate)
{
	m_onScrollCallback.push_back(delegate);
}
void Input::onCursorEnterCallback(const OnCursorEnterCallback &delegate)
{
	m_onCursorEnterCallback.push_back(delegate);
}
#pragma endregion //}

void Input::lateUpdate()
{
	for (auto &pair : m_keyStates)
	{
		if (pair.second.state == GLFW_PRESS)
			pair.second.state = GLFW_REPEAT;
		if (pair.second.state == GLFW_RELEASE)
			pair.second.state = GLFW_RELEASED;
	}

	m_prevCursorPos = m_cursorPos;
}

void Input::onKey(GLFWwindow *window, int key, int scanecode, int action, int mods)
{
	for (const auto &delegate : m_onKeyCallbacks)
		delegate(key, scanecode, action, mods);

	switch (action)
	{
	case GLFW_PRESS:
	case GLFW_RELEASE:
		m_keyStates[key].state = action;
		m_keyStates[key].mods = mods;
		break;

	default:
		m_keyStates[key].mods = mods;
		break;
	}
}

void Input::onCursorPos(GLFWwindow *window, double x, double y)
{
	for (const auto &delegate : m_onCursorPosCallback)
		delegate(x, y);

	m_cursorPos.x = x;
	m_cursorPos.y = y;
}

void Input::onMouseButton(GLFWwindow* window, int button, int action, int mods)
{
	for (const auto &delegate : m_onMouseButtonCallbacks)
		delegate(button, action, mods);

	switch (action)
	{
	case GLFW_PRESS:
	case GLFW_RELEASE:
		m_mouseButtonStates[button].state = action;
		m_mouseButtonStates[button].mods = mods;
		break;

	default:
		m_mouseButtonStates[button].mods = mods;
		break;
	}
}

void Input::onScroll(GLFWwindow* window, double x, double y)
{
	for (const auto &delegate : m_onScrollCallback)
		delegate(x, y);

	m_scrollPos.x = x;
	m_scrollPos.y = y;
}

void Input::onCursorEnter(GLFWwindow* window, int state)
{
	for (const auto &delegate : m_onCursorEnterCallback)
		delegate(state);
}

void Input::quit() { }

