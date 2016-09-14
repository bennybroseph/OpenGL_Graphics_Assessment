#ifndef _PLANE_H_
#define _PLANE_H_

#include "Shape.h"

class Plane : public Shape
{
public:

	Plane();

	static void init();

	static void genVertexes();
	static void genIndexes();

	~Plane();

private:

	static Model* s_plane;

};

#endif // _PLANE_H_