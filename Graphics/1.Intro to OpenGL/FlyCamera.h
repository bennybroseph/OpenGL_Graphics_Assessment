#ifndef _FLYCAMERA_H_
#define _FLYCAMERA_H_
#pragma once

#include "Camera.h"

class FlyCamera;

typedef unique_ptr<FlyCamera> FlyCameraPtrU;
typedef shared_ptr<FlyCamera> FlyCameraPtrS;
typedef weak_ptr<FlyCamera> FlyCameraPtrW;

class FlyCamera : public Camera
{
public:

	FlyCamera();

	void init() override;
	void update(const float &deltaTime) const override;

	~FlyCamera();

private:

	float m_speed = 5.f;
	const vec3PtrU m_up = make_unique<vec3>(0, 1, 0);

};

#endif // _FLYCAMERA_H_