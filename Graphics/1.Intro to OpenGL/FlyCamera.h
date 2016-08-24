#ifndef _FLYCAMERA_H_
#define _FLYCAMERA_H_

#include "Camera.h"

class FlyCamera : public Camera
{
public:
	
	FlyCamera();

	void update(const float &deltaTime) override;

	~FlyCamera() override;

private:

	float m_speed = 5.f;
	vec3 m_up = vec3(0, 1, 0);

};

#endif // _FLYCAMERA_H_