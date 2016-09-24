#include "Planet.h"



Planet::Planet(const vec3 &position, const float& radius, const vec4 &colour, const float &speed)
{
	m_transform->scale(vec3(radius, radius, radius));
	m_transform->setPosition(position);

	m_radius = radius;
	m_colour = colour;

	m_speed = speed;
}

void Planet::update(const float &deltaTime)
{
	m_transform->rotate(m_speed * deltaTime, vec3(0.f, 1.f, 0.f));
}

Transform& Planet::transform()
{
	return *m_transform;
}

float& Planet::radius()
{
	return m_radius;
}

vec4& Planet::colour()
{
	return m_colour;
}

Planet::~Planet() { }
