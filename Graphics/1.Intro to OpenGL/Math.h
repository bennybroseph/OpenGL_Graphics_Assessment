#ifndef _MATH_H_
#define _MATH_H_

#include <glm/ext.hpp>

using glm::vec3;

namespace Math
{
	int mod(int a, int b);
	float mod(float a, float b);

	float clampAngle(float angle);
	vec3 clampAngle(vec3 eulerAngle);
}


#endif // _MATH_H_
