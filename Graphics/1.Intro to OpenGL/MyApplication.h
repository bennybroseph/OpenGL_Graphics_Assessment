#ifndef _MY_APPLICATION_H_
#define _MY_APPLICATION_H_

#include "BaseApplication.h"
#include "Camera.h"

#include "Planet.h"
#include "Mesh.h"
#include "Light.h"

class MyApplication : public BaseApplication
{
public:

	int startup() override;

	~MyApplication();

private:

	// Inherited via BaseApplication
	void shutdown() override;
	void parseInput() override;
	void update() override;
	void lateUpdate() override;
	void draw() override;

	void drawSolarSystem();
	void drawGui();

	vec4 m_clearColour = vec4(0.25f, 0.25f, 0.25f, 1.f);

	vec2 m_screenSize = vec2(1600, 900);

	Camera *m_camera = nullptr;
	Light *m_light = nullptr;

	Planet *const m_sun = new Planet(vec3(0, 0, 0), 1.5f, vec4(255.f / 255.f, 235.f / 255.f, 59.f / 255.f, 1.f), 3.f);
	Planet *const m_earth = new Planet(vec3(5, 0, 0), 0.5f, vec4(139 / 255.f, 195 / 255.f, 74 / 255.f, 1.f), 10.f);
	Planet *const m_moon = new Planet(vec3(2, 0.5f, 0), 0.5f, vec4(0.9f, 0.9f, 0.9f, 1.f), -6.f);

	vector<Shape*> *const m_shapes = new vector<Shape*>();

	bool m_shouldDrawGrid = true;
	bool m_shouldDrawGui = true;

};

#endif // _MY_APPLICATION_H_