#ifndef _PLANE_H_
#define _PLANE_H_
#pragma once

#include "Model.h"

namespace Gizmos
{
	class Plane
	{

		const static GLfloat DEFAULT_SEGMENTS;

	public:

		static void init();

		static ModelPtrU createModel(float segments = DEFAULT_SEGMENTS);
		static const Mesh * getMesh(float segments = DEFAULT_SEGMENTS);

		static void quit();

	private:

		static string createKey(float segments);
		static bool createMeshIfNeeded(float segments = DEFAULT_SEGMENTS);

		static vectorPtrU<Vertex> genVertexes(float segments);
		static vectorPtrU<GLuint> genIndexes(float segments);

		static mapPtrU<string, MeshPtrU> s_meshes;

	};
}

#endif // _PLANE_H_