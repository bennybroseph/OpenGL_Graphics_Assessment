#ifndef _LINE_H_
#define _LINE_H_
#pragma once

#include "Model.h"

namespace Gizmos
{
	class Line
	{
	public:

		static void init();

		static ModelPtrU create(
			const vec3 &start,
			const vec3 &end,
			const vec4 &colourStart,
			const vec4 &colourEnd);

		static void quit();

	private:

		static vectorPtrU<Vertex> genVertexes(
			const vec3 &start,
			const vec3 &end,
			const vec4 &colourStart,
			const vec4 &colourEnd);
		static vectorPtrU<GLuint> genIndexes();

		static mapPtrU<string, MeshPtrU> s_meshes;

	};
}

#endif // _LINE_H_