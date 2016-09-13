#ifndef _BASE_APPLICATION_H_
#define _BASE_APPLICATION_H_

// ReSharper disable once CppUnusedIncludeDirective
// OpenGL requires a very specific include order
#include <gl_core_4_4.h>
#include <GLFW/glfw3.h>

#include "Input.h"
#include "Shader.h"

struct GLFWwindow;

class BaseApplication
{
public:
	/* Notes on virtual functions
	A virtual call is a mechanism to get work done given partial information.
	In particular, "virtual" allows us to call a function knowing only any interfaces and
	not the exact type of the object.
	To create an object you need complete information.
	In particular, you need to know the exact type of what you want to create.
	Consequently, a "call to a constructor" cannot be virtual.
	*/
	virtual ~BaseApplication() {}

	void run();
	// = 0 makes it a pure virtual function so you MUST inherit from this class and implement this function
	// in order to use it. It cannot be called if this is the object type
	virtual int startup() = 0;
	virtual void shutdown() = 0;

protected:

	virtual int createWindow(const char* title, int width, int height);
	virtual void destroyWindow();

	virtual void parseInput() = 0;

	virtual void update() = 0;
	virtual void lateUpdate() = 0;

	virtual void draw() = 0;

	GLFWwindow*	m_window = nullptr;
	bool m_isRunning = true;

	float m_deltaTime = 0.f;
};
#endif // _BASE_APPLICATION_H_
