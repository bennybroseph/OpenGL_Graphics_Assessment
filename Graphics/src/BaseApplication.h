#pragma once

struct GLFWwindow;

class BaseApplication {
public:

	BaseApplication() {}
	virtual ~BaseApplication() {}

	void run(); 
	virtual bool startup() = 0;
	virtual void shutdown() = 0;

	virtual bool update(float deltaTime) = 0;
	virtual void draw() = 0;

protected:

	virtual bool createWindow(const char* title, int width, int height);
	virtual void destroyWindow();

	void setInputCallback(void(*func)(GLFWwindow* window, int key, int scancode, int action, int mods));

	GLFWwindow*	m_window;
};