#ifndef _PLANE_H_
#define _PLANE_H_

#include "Shape3D.h"

class Plane : public Shape3D
{
public:

	static void init();

	static void genVertexes();
	static void genIndexes();

	~Plane();

};

#endif // _PLANE_H_

