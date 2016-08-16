#ifndef _MY_APPLICATION_H_
#define _MY_APPLICATION_H_

#include <glm/ext.hpp>
#include "BaseApplication.h"

class MyApplication : public BaseApplication {
public:

	int startup() override;

private:

	// Inherited via BaseApplication
	void shutdown() override;
	void parseInput() override;
	void update() override;
	void lateUpdate() override;
	void draw() override;

	void drawSolarSystem();
	void drawGrid();

	glm::mat4 m_view;
	glm::mat4 m_projection;

	glm::mat4 m_sun;
	glm::mat4 m_earth;

	bool m_shouldDrawGrid = true;
	int m_prevF1State = GLFW_RELEASE;
};

#endif // _MY_APPLICATION_H_