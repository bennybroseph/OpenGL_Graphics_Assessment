#ifndef _DIRECTIONAL_LIGHT_H_
#define _DIRECTIONAL_LIGHT_H_
#pragma once

#include "Light.h"

class DirectionalLight;

typedef unique_ptr<DirectionalLight> DirectionalLightPtrU;
typedef shared_ptr<DirectionalLight> DirectionalLightPtrS;
typedef weak_ptr<DirectionalLight> DirectionalLightPtrW;

class DirectionalLight : public Light
{
public:

	DirectionalLight();

	void draw() const override;
	void drawGui() const override;

	~DirectionalLight();

private:

};

#endif // _DIRECTIONALLIGHT_H_

