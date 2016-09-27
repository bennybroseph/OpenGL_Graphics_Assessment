#ifndef _SPHERE_H_
#define _SPHERE_H_
#pragma once

#include "Shape.h"

namespace Gizmos
{
	class Sphere
	{
	public:

		static void init();

		static ModelPtrU create();

		static void quit();

	private:

		static void genVertexes(const float &radius, const float &segments);
		static vector<Vertex> genHalfCircle(const float &radius, const float &segments);
		static void genIndexes(const float &radius, const float &segments);

		static vectorPtrU<Vertex> m_vertexes;
		static vectorPtrU<GLuint> m_indexes;

	};
}

#endif // _SPHERE_H_