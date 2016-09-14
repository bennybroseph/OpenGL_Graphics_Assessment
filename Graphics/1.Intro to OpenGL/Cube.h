#ifndef _CUBE_H_
#define _CUBE_H_

#include "Shape.h"

class Cube : public Shape
{
public:

	Cube();

	static void init();

	static void genVertexes();
	static void genIndexes();

	~Cube();

private:

	static Model* s_cube;

};

#endif // _CUBE_H_