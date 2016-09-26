#ifndef _INPUT_H_
#define _INPUT_H_
#pragma once

#include <GLFW/glfw3.h>

#include "Delegate.h"

#include "MasterHeader.h"

#define GLFW_RELEASED -1

typedef Delegate<void, GLFWwindow *, GLint, GLint, GLint, GLint> OnKeyCallback;
typedef Delegate<void, GLFWwindow *, GLdouble, GLdouble> OnCursorPosCallback;
typedef Delegate<void, GLFWwindow *, GLint, GLint, GLint> OnMouseButtonCallback;
typedef Delegate<void, GLFWwindow *, GLdouble, GLdouble> OnScrollCallback;
typedef Delegate<void, GLFWwindow *, GLint> OnCursorEnterCallback;

// Static class vs. Namespace
// This is a static class because I need to hide some member functions from outside users
// To me, this is the simplest way to do it.
// Syntax from a namespace and static class are exactly the same, so no big deal
class Input
{
public:

	struct KeyState
	{
		GLint state = GLFW_RELEASED;
		GLint mods = 0x0;
	};
	typedef unique_ptr<KeyState> KeyStatePtr;

	static void init();

	static GLint getKey(GLint key);
	static GLint getKey(GLint key, GLint mods);

	static const glm::dvec2 & getCursorPosition();
	static glm::dvec2 deltaCursorPosition();

	static GLint getMouseButton(GLint button);
	static GLint getMouseButton(GLint button, GLint mods);

	static glm::dvec2 getScrollPosition();

	static void addOnKeyCallback(const OnKeyCallback &delegate);
	static void addOnCursorPosCallback(const OnCursorPosCallback &delegate);
	static void addOnMouseButtonCallback(const OnMouseButtonCallback &delegate);
	static void addOnScrollCallback(const OnScrollCallback &delegate);
	static void addOnCursorEnterCallback(const OnCursorEnterCallback &delegate);

	static void lateUpdate();

	static void quit();

private:

	static void onKey(GLFWwindow *window, GLint key, GLint scanecode, GLint action, GLint mods);
	static void onCursorPos(GLFWwindow *window, GLdouble x, GLdouble y);
	static void onMouseButton(GLFWwindow *window, GLint button, GLint action, GLint mods);
	static void onScroll(GLFWwindow *window, GLdouble x, GLdouble y);
	static void onCursorEnter(GLFWwindow *window, GLint state);

	Input();

	static vectorPtrU<OnKeyCallback> m_onKeyCallbacks;
	static vectorPtrU<OnCursorPosCallback> m_onCursorPosCallback;
	static vectorPtrU<OnMouseButtonCallback> m_onMouseButtonCallbacks;
	static vectorPtrU<OnScrollCallback> m_onScrollCallback;
	static vectorPtrU<OnCursorEnterCallback> m_onCursorEnterCallback;

	static unique_ptr<Input> self;

	// Since this is a singleton allocated on the heap,
	// there is no need to allocate any members dynamically
	map<GLint, KeyState> m_keyStates = map<GLint, KeyState>();

	glm::dvec2 m_cursorPos = glm::dvec2();
	glm::dvec2 m_prevCursorPos = glm::dvec2();

	map<GLint, KeyState> m_mouseButtonStates = map<GLint, KeyState>();

	glm::dvec2 m_scrollPos = glm::dvec2();
	glm::dvec2 m_prevScrollPos = glm::dvec2();

};

#endif // INPUT_H_