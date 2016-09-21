#include "Light.h"

vector<Light*> Light::s_lights =  vector<Light*>();

Light::Light()
{
	s_lights.push_back(this);
}

Light::~Light()
{
	for (auto light : s_lights)
		delete light;
}
