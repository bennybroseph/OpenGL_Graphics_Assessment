#include "Math.h"



namespace Math
{
	int mod(int a, int b)
	{
		b = abs(b);

		auto returnValue = a % b;
		if (returnValue <= 0)
			returnValue += b;

		return returnValue;
	}
	float mod(float a, float b)
	{
		auto decimalValue = a - int(a);

		return mod(int(a), int(b)) + decimalValue;
	}

	float clampAngle(float angle)
	{
		return mod(angle, 360.f);
	}
	vec3 clampAngle(vec3 eulerAngle)
	{
		clampAngle(eulerAngle.x);
		clampAngle(eulerAngle.y);
		clampAngle(eulerAngle.z);

		return eulerAngle;
	}
}
