#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "Transform.h"
#include <vector>

using std::vector;

class Light
{
public:

	Light();

	virtual void draw() const = 0;
	virtual void drawGui(const char *title = "Light") const = 0;

	virtual ~Light();

	static vector<Light*> *const s_lights;

	Transform *const m_transform = new Transform();

	vec3 *const m_direction = new vec3(0.f);

	vec3 *const m_diffuse = new vec3(1.f);
	vec3 *const m_specular = new vec3(1.f);

};

#endif // _LIGHT_H_
