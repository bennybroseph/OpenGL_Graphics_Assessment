#include "Plane.h"


void Plane::init()
{
	genVertexes();
	genIndexes();
	genBuffers();
}

void Plane::genVertexes()
{
	s_vertexes.clear();

	s_vertexes.push_back(
	{
		vec4(-1.f, 0.f, -1.f, 1.f),
		vec4(1.f, 1.f, 1.f, 1.f)
	});
	s_vertexes.push_back(
	{
		vec4(1.f, 0.f, -1.f, 1.f),
		vec4(1.f, 1.f, 1.f, 1.f)
	});
	s_vertexes.push_back(
	{
		vec4(-1.f, 0.f, 1.f, 1.f),
		vec4(1.f, 1.f, 1.f, 1.f)
	});
	s_vertexes.push_back(
	{
		vec4(1.f, 0.f, 1.f, 1.f),
		vec4(1.f, 1.f, 1.f, 1.f)
	});
}

void Plane::genIndexes()
{
	s_indexes.clear();

	s_indexes.push_back(0);
	s_indexes.push_back(2);
	s_indexes.push_back(1);
	s_indexes.push_back(3);
}

Plane::~Plane()
{
}
