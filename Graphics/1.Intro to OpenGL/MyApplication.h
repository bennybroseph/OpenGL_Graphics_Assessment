#ifndef _MY_APPLICATION_H_
#define _MY_APPLICATION_H_
#pragma once

#include "BaseApplication.h"
#include "Camera.h"

#include "Planet.h"
#include "Model.h"
#include "Light.h"
#include "GameObject.h"

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

	void drawSolarSystem() const;
	void drawGui();

	vec4 m_clearColour = vec4(0.25f, 0.25f, 0.25f, 1.f);

	vec2 m_screenSize = vec2(1600, 900);

	CameraPtrU m_camera = nullptr;
	LightPtrU m_light = nullptr;

	GameObjectPtrU m_sun = nullptr;
	GameObjectPtrU m_earth = nullptr;
	GameObjectPtrU m_moon = nullptr;

	const vectorPtrU<GameObjectPtrU> m_shapes = make_unique<vector<GameObjectPtrU>>();

	bool m_shouldDrawGrid = true;
	bool m_shouldDrawGui = true;

};

#endif // _MY_APPLICATION_H_