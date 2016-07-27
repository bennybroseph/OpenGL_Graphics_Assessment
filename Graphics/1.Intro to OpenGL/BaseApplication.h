#pragma once
#include <gl_core_4_4.h>
#include <GLFW/glfw3.h>
#include <iostream>


//Finish the following prototype functions

struct GLFWwindow;

class BaseApplication {
public:
	/* Notes on virtual functions
	A virtual call is a mechanism to get work done given partial information. 
	In particular, "virtual" allows us to call a function knowing only any interfaces and not the exact type of the object. 
	To create an object you need complete information. 
	In particular, you need to know the exact type of what you want to create. Consequently, a "call to a constructor" cannot be virtual.
	*/

	BaseApplication() {}
	virtual ~BaseApplication() {} 

	void run();
	virtual bool startup() = 0; // = 0 makes it a pure virtual function so you MUST derive a class (and implement said function) in order to use it
	virtual void shutdown() = 0;

	virtual bool update(float deltaTime) = 0;
	virtual void draw() = 0;

protected:

	virtual bool createWindow(const char* title, int width, int height);
	virtual void destroyWindow();

	GLFWwindow*	m_window;
};

