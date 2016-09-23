#ifndef _DIRECTIONAL_LIGHT_H_
#define _DIRECTIONAL_LIGHT_H_

#include "Light.h"

class DirectionalLight : public Light
{
public:

	void draw() override;
	void drawGui(const char *title = "Directional Light") override;

	~DirectionalLight();

private:


};

#endif // _DIRECTIONALLIGHT_H_

