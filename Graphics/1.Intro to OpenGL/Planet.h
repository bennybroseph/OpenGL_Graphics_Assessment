#pragma once

#include "Transform.h"

class Planet
{
public:

	Planet();
	explicit Planet(const vec3 &position, const float& radius, const vec4 &colour, const float &speed = 0.005f);

	void update();
	void lateUpdate();

	Transform& getTransform();

	float& radius();
	vec4& color();

	~Planet();

private:

	Transform m_transform = Transform();

	float m_radius = 1.f;
	vec4 m_colour = vec4(0, 0, 0, 1);

	float m_speed = 0.001f;
};

