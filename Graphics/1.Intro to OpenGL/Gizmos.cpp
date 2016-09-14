#include "Gizmos.h"



namespace Gizmos
{
	int init()
	{
		glLineWidth(2.0f);

		Plane::init();
		Cube::init();
		Sphere::init();

		return 0;
	}

	int drawSphere(const mat4 &transform, const float &radius, const vec4 &colour, const bool &drawWireFrame)
	{
		auto newSphere = Sphere();

		newSphere.transform().scale(vec3(radius, radius, radius));
		newSphere.transform().setLocalSpaceMatrix(transform);

		newSphere.materialColour() = colour;

		newSphere.shouldDrawWireFrame() = drawWireFrame;

		newSphere.draw();

		return 0;
	}

	int quit()
	{
		return 0;
	}
}
