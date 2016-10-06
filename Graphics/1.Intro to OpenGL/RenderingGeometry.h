#pragma once

#include "BaseApplication.h"
#include "GameObject.h"

class RenderingGeometry  : public BaseApplication
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

	GameObjectPtrU m_camera = nullptr;

	GameObjectPtrU m_plane = nullptr;

};

