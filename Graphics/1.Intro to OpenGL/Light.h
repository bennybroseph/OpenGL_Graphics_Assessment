#ifndef _LIGHT_H_
#define _LIGHT_H_
#pragma once

#include "Transform.h"

#include "MasterHeader.h"

class Light;

typedef unique_ptr<Light> LightPtrU;
typedef shared_ptr<Light> LightPtrS;
typedef weak_ptr<Light> LightPtrW;

class Light
{
public:

	Light();

	virtual void draw() const = 0;
	virtual void drawGui(const char *title = "Light") const = 0;

	virtual ~Light();

	static vectorPtrU<Light *> s_lights;

	const TransformPtrU m_transform = make_unique<Transform>();

	const vec3PtrU m_direction = make_unique<vec3>(0.f);

	const vec3PtrU  m_diffuse = make_unique<vec3>(1.f);
	const vec3PtrU m_specular = make_unique<vec3>(1.f);

};

#endif // _LIGHT_H_
