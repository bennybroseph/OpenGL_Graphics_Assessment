#ifndef _CUBE_H_
#define _CUBE_H_
#pragma once

#include "Shape.h"

class Cube : public Shape
{
public:

	Cube();

	~Cube();

	static void init();

	static void genVertexes();
	static void genIndexes();

	static void quit();

private:

	static MeshPtrU s_cube;

};

#endif // _CUBE_H_