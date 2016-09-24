#ifndef _FLYCAMERA_H_
#define _FLYCAMERA_H_

#include "Camera.h"

class FlyCamera : public Camera
{
public:

	FlyCamera();

	void update(const float &deltaTime) const override;

	~FlyCamera() override;

private:

	float *const m_speed = new float(5.f);
	vec3 *const m_up = new vec3(0, 1, 0);

};

#endif // _FLYCAMERA_H_