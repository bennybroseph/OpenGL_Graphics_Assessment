#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "Transform.h"

class Light
{
public:

	virtual void draw() = 0;

	virtual ~Light();

protected:

	Transform m_transform;

	vec3 m_direction = vec3(0);

	vec3 diffuse = vec3(1);
	vec3 specular = vec3(1);

};

#endif // _LIGHT_H_
