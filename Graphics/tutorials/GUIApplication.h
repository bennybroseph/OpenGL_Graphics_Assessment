#pragma once

#include "BaseApplication.h"

#include <glm/vec4.hpp>

// forward declared to reduce dependencies
class Camera;

class GUIApplication : public BaseApplication {
public:

	GUIApplication();
	virtual ~GUIApplication();

	virtual bool startup();
	virtual void shutdown();

	virtual bool update(float deltaTime);
	virtual void draw();

private:

	Camera*		m_camera;

	glm::vec4	m_clearColour;
	bool		m_drawGrid;
};