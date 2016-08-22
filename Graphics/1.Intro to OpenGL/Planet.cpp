#include "Planet.h"



Planet::Planet() { }

Planet::Planet(const vec3 &position, const float& radius, const vec4 &colour, const float &speed)
{
	m_transform.setPosition(position);
	m_radius = radius;
	m_colour = colour;

	m_speed = speed;
}

void Planet::update(float deltaTime)
{
	m_transform.rotate(m_speed * deltaTime, vec3(0, 1, 0));
}

Transform& Planet::transform()
{
	return m_transform;
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