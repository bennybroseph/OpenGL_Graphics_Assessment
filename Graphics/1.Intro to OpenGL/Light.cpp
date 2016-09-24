#include "Light.h"

vector<Light*> *const Light::s_lights = new vector<Light*>();

Light::Light()
{
	s_lights->push_back(this);
}

Light::~Light()
{
	if (m_transform != nullptr)
		delete m_transform;

	if (m_direction != nullptr)
		delete m_direction;

	if (m_diffuse != nullptr)
		delete m_diffuse;
	if (m_specular != nullptr)
		delete m_specular;
}
