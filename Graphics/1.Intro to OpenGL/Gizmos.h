#ifndef _GIZMOS_H_
#define _GIZMOS_H_

#include "Plane.h"
#include "Cube.h"
#include "Sphere.h"

namespace Gizmos
{
	int init();

	int drawSphere(const mat4 &transform, const vec4 &colour, const bool &drawWireFrame = false);
	// TODO: This function is so expensive...please only buffer once somehow
	int drawLine(const vec3 &start, const vec3 &end, const vec4 &colour, const GLfloat &width = 1.f);

	int drawGrid(
		const vec3 &center,
		const vec2 &size,
		const vec2 &segments,
		const vec4 &colour,
		const GLfloat &lineWidth = 1.f);

	int quit();
}

#endif // _GIZMOS_H_