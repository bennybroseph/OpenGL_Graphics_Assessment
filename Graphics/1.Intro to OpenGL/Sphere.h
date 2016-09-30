#ifndef _SPHERE_H_
#define _SPHERE_H_
#pragma once

#include "Model.h"

namespace Gizmos
{
	class Sphere
	{

		const static GLfloat DEFAULT_RADIUS;
		const static GLfloat DEFAULT_SEGMENTS;

	public:

		static void init();

		static ModelPtrU create(float radius = DEFAULT_RADIUS, float segments = DEFAULT_SEGMENTS);
		static const Mesh * getMesh(float radius = DEFAULT_RADIUS, float segments = DEFAULT_SEGMENTS);

		static void quit();

	private:

		static string createKey(float radius, float segments);
		static bool createMeshIfNeeded(float radius = DEFAULT_RADIUS, float segments = DEFAULT_SEGMENTS);

		static vectorPtrU<Vertex> genVertexes(const float &radius, const float &segments);
		static vector<Vertex> genHalfCircle(const float &radius, const float &segments);
		static vectorPtrU<GLuint> genIndexes(const float &radius, const float &segments);

		static mapPtrU<string, MeshPtrU> s_meshes;

	};
}

#endif // _SPHERE_H_