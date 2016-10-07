#ifndef _INTROTOOPENGL_H_
#define _INTROTOOPENGL_H_
#pragma once

#include "BaseApplication.h"
#include "GameObject.h"

class IntroToOpenGL : public BaseApplication
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

};


#endif // _INTROTOOPENGL_H_
