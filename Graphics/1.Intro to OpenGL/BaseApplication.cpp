#include "BaseApplication.h"

bool BaseApplication::createWindow(const char * title, int width, int height)
{
	if (glfwInit() == false)
		return -1;

	m_window = glfwCreateWindow(1280, 720, "Computer Graphics", nullptr, nullptr);

	if (m_window == nullptr)
	{
		glfwTerminate();
		return -2;
	}

	glfwMakeContextCurrent(m_window);
	// the rest of our code goes here!
	if (ogl_LoadFunctions() == ogl_LOAD_FAILED)
	{
		glfwDestroyWindow(m_window);
		glfwTerminate(); //misspelled glfwterminate()
		return -3;
	}

	auto major = ogl_GetMajorVersion();
	auto minor = ogl_GetMinorVersion();
	printf("GL: %i.%i\n", major, minor);
	glClearColor(0.25f, 0.25f, 0.25f, 1);
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
	float prevTime = glfwGetTime();
	float currTime = 0;
	while (currTime = glfwGetTime(), update((currTime - prevTime)))//wat
	{
		glfwPollEvents();
		draw(); //call the implemented draw function of whatever application we have designated
		glfwSwapBuffers(m_window);
		prevTime = currTime;
	}
}
 
