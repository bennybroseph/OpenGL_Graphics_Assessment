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

	Camera* m_camera = nullptr;
	Light* m_light = nullptr;

	Planet m_sun;
	Planet m_earth;
	Planet m_moon;

	vector<Shape*> m_shapes = vector<Shape*>();

	bool m_shouldDrawGrid = true;
	bool m_shouldDrawGui = true;

};

#endif // _MY_APPLICATION_H_