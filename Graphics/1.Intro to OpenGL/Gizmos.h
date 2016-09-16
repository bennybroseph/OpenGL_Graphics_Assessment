#ifndef _GIZMOS_H_
#define _GIZMOS_H_

#include "Plane.h"
#include "Cube.h"
#include "Sphere.h"

namespace Gizmos
{
	int init();

	int drawSphere(const mat4 &transform, const vec4 &colour, const bool &drawWireFrame);

	int quit();
}

#endif // _GIZMOS_H_