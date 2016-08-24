#ifndef _MY_APPLICATION_H_
#define _MY_APPLICATION_H_

#include "BaseApplication.h"
#include "Camera.h"

#include "Planet.h"

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

	Camera* m_camera;

	Planet m_sun;
	Planet m_earth;
	Planet m_moon;

	bool m_shouldDrawGrid = true;

};

#endif // _MY_APPLICATION_H_