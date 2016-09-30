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

		static ModelPtrU createModel();
		static const Mesh * getMesh();

		static void quit();

	private:

		static void genVertexes();
		static void genIndexes();

		static MeshPtrU s_mesh;

	};
}

#endif // _PLANE_H_