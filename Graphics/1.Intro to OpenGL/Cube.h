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
		static const Mesh * getMesh();

		static void quit();

	private:

		static void genVertexes();
		static void genIndexes();

		static MeshPtrU s_mesh;

	};
}

#endif // _CUBE_H_