#include "Cube.h"


void Cube::init()
{
	genVertexes();
	genIndexes();
	genBuffers();
}

void Cube::genVertexes()
{
	s_vertexes.clear();

	s_vertexes.push_back(
	{
		vec4(-1.f, -1.f, 1.f, 1.f),		// Front-Bottom-Left
		vec4(1.f, 0.f, 0.f, 1.f)
	});
	s_vertexes.push_back(
	{
		vec4(1.f, -1.f, 1.f, 1.f),		// Front-Bottom-Right
		vec4(1.f, 0.5f, 0.f, 1.f)
	});
	s_vertexes.push_back(
	{
		vec4(-1.f, 1.f, 1.f, 1.f),		// Front-Top-Left
		vec4(1.f, 0.f, 0.f, 1.f)
	});	
	s_vertexes.push_back(
	{
		vec4(1.f, 1.f, 1.f, 1.f),		// Front-Top-Right
		vec4(0.f, 1.f, 0.f, 1.f)
	});
	s_vertexes.push_back(
	{
		vec4(-1.f, -1.f, -1.f, 1.f),	// Back-Bottom-Left
		vec4(0.f, 1.f, 1.f, 1.f)
	});
	s_vertexes.push_back(
	{
		vec4(1.f, -1.f, -1.f, 1.f),		// Back-Bottom-Right
		vec4(0.f, 1.f, 1.f, 1.f)
	});
	s_vertexes.push_back(
	{
		vec4(-1.f, 1.f, -1.f, 1.f),		// Back-Top-Left
		vec4(0.f, 0.f, 1.f, 1.f)
	});
	s_vertexes.push_back(
	{
		vec4(1.f, 1.f, -1.f, 1.f),		// Back-Top-Right
		vec4(1.f, 0.f, 1.f, 1.f)
	});	
}

void Cube::genIndexes()
{
	s_indexes.clear();

	s_indexes.push_back(7);
	s_indexes.push_back(6);
	s_indexes.push_back(3);
	s_indexes.push_back(2);
	s_indexes.push_back(0);
	s_indexes.push_back(6);
	s_indexes.push_back(4);
	s_indexes.push_back(7);
	s_indexes.push_back(5);
	s_indexes.push_back(3);
	s_indexes.push_back(1);
	s_indexes.push_back(0);
	s_indexes.push_back(5);
	s_indexes.push_back(4);
}

Cube::~Cube()
{
}
