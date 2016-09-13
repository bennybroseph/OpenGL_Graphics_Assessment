#ifndef _CUBE_H_
#define _CUBE_H_

#include "Shape3D.h"

class Cube : public Shape3D
{
public:

	static void init();

	static void genVertexes();
	static void genIndexes();

	~Cube();

};

#endif // _CUBE_H_

