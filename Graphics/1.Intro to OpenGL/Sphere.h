#ifndef _SPHERE_H_
#define _SPHERE_H_

#define PI glm::pi<float>()

#include <float.h>

#include "Shape.h"

class Sphere : public Shape
{
public:

	Sphere();

	static void init();

	static void genVertexes(const float &radius, const float &segments);

	static vector<Vertex> genHalfCircle(const float &radius, const float &segments);

	static void genIndexes(const float &radius, const float &segments);

	~Sphere();

private:

	static Mesh* s_sphere;

};

#endif // _SPHERE_H_