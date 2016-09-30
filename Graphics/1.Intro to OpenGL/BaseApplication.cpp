#include "BaseApplication.h"

#include <cstdio>

int BaseApplication::createWindow(const char * title, int width, int height)
{
	if (glfwInit() == false)
		return -1;

	m_window = glfwCreateWindow(width, height, title, nullptr, nullptr);

	if (m_window == nullptr)
	{
		glfwTerminate();
		return -2;
	}

	glfwMakeContextCurrent(m_window);
	glfwSwapInterval(0);

	// the rest of our code goes here!
	if (ogl_LoadFunctions() == ogl_LOAD_FAILED)
	{
		glfwDestroyWindow(m_window);
		glfwTerminate();
		return -3;
	}

	auto major = ogl_GetMajorVersion();
	auto minor = ogl_GetMinorVersion();
	printf("GL: %i.%i\n", major, minor);

	// Enable alpha transparency drawing
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glClearColor(0.25f, 0.25f, 0.25f, 1.f);
	glEnable(GL_DEPTH_TEST); // enables the depth buffer

	return false;
}

void BaseApplication::destroyWindow()
{
	glfwDestroyWindow(m_window);
	glfwTerminate();
}

void BaseApplication::run()
{
	auto currTime = glfwGetTime();
	double prevTime;

	// set currTime to glfwGetTime() and run the update() function
	// the loop runs so long as update returns true
	auto fps = 0;
	auto time = 0.f;
	while (m_isRunning)
	{
		prevTime = currTime;
		currTime = glfwGetTime();
		m_deltaTime = static_cast<float>(currTime - prevTime);

		glfwPollEvents();
		parseInput();

		if (!m_isRunning)
			continue;

		update();
		lateUpdate();


		draw(); //call the implemented draw function of whatever application we have designated

		/*time += m_deltaTime;
		fps++;

		if(time >= 1.f)
		{
			printf("\n%i", fps);

			fps = 0;
			time = 0;
		}*/

		glfwSwapBuffers(m_window);
	}
}

