#include "Light.h"

vectorPtrU<Light *> Light::s_lights = make_unique<vector<Light *>>();

Light::Light()
{
	if (s_lights.get() == nullptr)
		s_lights.reset(new vector<Light *>());

	s_lights->push_back(this);	// Add itself to the list of lights
}

Light::~Light()
{
	s_lights->erase(find(s_lights->begin(), s_lights->end(), this));	// Remove itself from the list of lights

	if (s_lights->size() == 0)
		s_lights.reset();
}
