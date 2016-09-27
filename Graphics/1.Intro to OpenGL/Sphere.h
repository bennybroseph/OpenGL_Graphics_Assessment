#ifndef _SPHERE_H_
#define _SPHERE_H_
#pragma once

#include "Shape.h"

class Sphere : public Shape
{
public:

	Sphere();

	~Sphere();

	static void init();

	static void genVertexes(const float &radius, const float &segments);
	static vector<Vertex> genHalfCircle(const float &radius, const float &segments);
	static void genIndexes(const float &radius, const float &segments);

	static void quit();

private:

	static MeshPtrS s_sphere;

};

#endif // _SPHERE_H_