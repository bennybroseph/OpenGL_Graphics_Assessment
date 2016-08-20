#include "Planet.h"



Planet::Planet() { }

Planet::Planet(const vec3 &position, const float& radius, const vec4 &colour, const float &speed)
{
	m_transform.setPosition(position);
	m_radius = radius;
	m_colour = colour;

	m_speed = speed;
}

void Planet::update()
{
	m_transform.rotate(m_speed, vec3(0, 1, 0));
}

void Planet::lateUpdate()
{
	m_transform.update();
}

Transform& Planet::getTransform()
{
	return m_transform;
}

float& Planet::radius()
{
	return m_radius;
}

vec4& Planet::color()
{
	return m_colour;
}

Planet::~Planet() { }
