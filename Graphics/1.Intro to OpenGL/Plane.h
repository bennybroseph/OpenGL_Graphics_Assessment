#ifndef _PLANE_H_
#define _PLANE_H_
#pragma once

#include "Shape.h"

class Plane : public Shape
{
public:

	Plane();

	~Plane();

	static void init();

	static void genVertexes();
	static void genIndexes();

	static void quit();

private:

	static MeshPtrU s_plane;

};

#endif // _PLANE_H_