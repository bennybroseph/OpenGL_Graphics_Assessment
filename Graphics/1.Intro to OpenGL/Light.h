#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "Transform.h"
#include <vector>

using std::vector;

class Light
{
public:

	Light();

	virtual void draw() = 0;

	virtual ~Light();

	static vector<Light*> s_lights;

	Transform m_transform;

	vec3 m_direction = vec3(0.f);

	vec3 m_diffuse = vec3(1.f);
	vec3 m_specular = vec3(1.f);

};

#endif // _LIGHT_H_
