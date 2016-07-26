#pragma once

#include "BaseApplication.h"

// forward declared to reduce dependencies
class Camera;

class SolarSystemApplication : public BaseApplication {
public:

	SolarSystemApplication();
	virtual ~SolarSystemApplication();

	virtual bool startup();
	virtual void shutdown();

	virtual bool update(float deltaTime);
	virtual void draw();

	static void inputCallback(GLFWwindow* window, int key, int scanline, int action, int mods);

private:

	Camera*		m_camera;

	// we can toggle the way the earth orbits the sun,
	// demonstating input callback
	bool		m_direction;
};