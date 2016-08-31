#ifndef _MY_APPLICATION_H_
#define _MY_APPLICATION_H_

#include "BaseApplication.h"
#include "Camera.h"

#include "Planet.h"

struct Vertex;

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
	void generateGrid();

	Camera* m_camera;

	Planet m_sun;
	Planet m_earth;
	Planet m_moon;

	unsigned int m_programID;

	unsigned int m_VAO;
	unsigned int m_VBO;
	unsigned int m_IBO;

	bool m_shouldDrawGrid = true;

};

struct Vertex
{
	vec4 position;
	vec4 colour;
};

#endif // _MY_APPLICATION_H_