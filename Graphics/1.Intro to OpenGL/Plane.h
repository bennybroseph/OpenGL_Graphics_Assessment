#ifndef _PLANE_H_
#define _PLANE_H_
#pragma once

#include "Model.h"

namespace Gizmos
{
	class Plane
	{
	public:

		static void init();

		static ModelPtrU create();

		static void quit();

	private:

		static void genVertexes();
		static void genIndexes();

		static vectorPtrU<Vertex> m_vertexes;
		static vectorPtrU<GLuint> m_indexes;

	};
}

#endif // _PLANE_H_