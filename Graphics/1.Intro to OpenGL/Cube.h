#ifndef _CUBE_H_
#define _CUBE_H_
#pragma once

#include "Model.h"

namespace Gizmos
{
	class Cube
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

#endif // _CUBE_H_