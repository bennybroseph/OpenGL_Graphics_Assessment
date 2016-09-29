#ifndef _SPHERE_H_
#define _SPHERE_H_
#pragma once

#include "Model.h"

namespace Gizmos
{
	class Sphere
	{
	public:

		static void init();

		static ModelPtrU create(float radius = 0.5f, float segments = 20.f);

		static void quit();

	private:

		static vectorPtrU<Vertex> genVertexes(const float &radius, const float &segments);
		static vector<Vertex> genHalfCircle(const float &radius, const float &segments);
		static vectorPtrU<GLuint> genIndexes(const float &radius, const float &segments);

		static mapPtrU<string, MeshPtrU> s_meshes;

	};
}

#endif // _SPHERE_H_