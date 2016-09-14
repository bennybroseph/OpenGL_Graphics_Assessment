#ifndef _MY_APPLICATION_H_
#define _MY_APPLICATION_H_

#include "BaseApplication.h"
#include "Camera.h"

#include "Planet.h"
#include "Model.h"

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

	Camera* m_camera = nullptr;

	Planet m_sun;
	Planet m_earth;
	Planet m_moon;

	vector<Shape*> m_shapes = vector<Shape*>();

	bool m_shouldDrawGrid = true;

};

#endif // _MY_APPLICATION_H_