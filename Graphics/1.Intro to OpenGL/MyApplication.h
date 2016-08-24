#ifndef _MY_APPLICATION_H_
#define _MY_APPLICATION_H_

#include <glm/ext.hpp>

#include "BaseApplication.h"
#include "Planet.h"

using glm::vec3;
using glm::vec4;
using glm::mat4;

class MyApplication : public BaseApplication
{
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
	void drawGrid() const;

	mat4 m_view;
	mat4 m_projection;

	Planet m_sun;
	Planet m_earth;
	Planet m_moon;

	bool m_shouldDrawGrid = true;
};

#endif // _MY_APPLICATION_H_