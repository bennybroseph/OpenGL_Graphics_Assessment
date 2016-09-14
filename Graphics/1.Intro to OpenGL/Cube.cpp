#include "Cube.h"

Model* Cube::s_cube = new Model();

Cube::Cube()
{
	m_model = s_cube;
	m_drawType = GL_TRIANGLE_STRIP;

	m_shouldDrawWireFrame = true;
}

void Cube::init()
{
	genVertexes();
	genIndexes();

	s_cube->genBuffers();
}

void Cube::genVertexes()
{
	s_cube->m_vertexes.clear();

	s_cube->m_vertexes.push_back(
	{
		vec4(-0.5f, -0.5f, 0.5f, 1.f),		// Front-Bottom-Left
		vec4(1.f, 1.f, 1.f, 1.f)
	});
	s_cube->m_vertexes.push_back(
	{
		vec4(0.5f, -0.5f, 0.5f, 1.f),		// Front-Bottom-Right
		vec4(1.f, 1.f, 1.f, 1.f)
	});
	s_cube->m_vertexes.push_back(
	{
		vec4(-0.5f, 0.5f, 0.5f, 1.f),		// Front-Top-Left
		vec4(1.f, 1.f, 1.f, 1.f)
	});
	s_cube->m_vertexes.push_back(
	{
		vec4(0.5f, 0.5f, 0.5f, 1.f),		// Front-Top-Right
		vec4(1.f, 1.f, 1.f, 1.f)
	});
	s_cube->m_vertexes.push_back(
	{
		vec4(-0.5f, -0.5f, -0.5f, 1.f),		// Back-Bottom-Left
		vec4(1.f, 1.f, 1.f, 1.f)
	});
	s_cube->m_vertexes.push_back(
	{
		vec4(0.5f, -0.5f, -0.5f, 1.f),		// Back-Bottom-Right
		vec4(1.f, 1.f, 1.f, 1.f)
	});
	s_cube->m_vertexes.push_back(
	{
		vec4(-0.5f, 0.5f, -0.5f, 1.f),		// Back-Top-Left
		vec4(1.f, 1.f, 1.f, 1.f)
	});
	s_cube->m_vertexes.push_back(
	{
		vec4(0.5f, 0.5f, -0.5f, 1.f),		// Back-Top-Right
		vec4(1.f, 1.f, 1.f, 1.f)
	});
}

void Cube::genIndexes()
{
	s_cube->m_indexes.clear();

	s_cube->m_indexes.push_back(7);
	s_cube->m_indexes.push_back(6);
	s_cube->m_indexes.push_back(3);
	s_cube->m_indexes.push_back(2);
	s_cube->m_indexes.push_back(0);
	s_cube->m_indexes.push_back(6);
	s_cube->m_indexes.push_back(4);
	s_cube->m_indexes.push_back(7);
	s_cube->m_indexes.push_back(5);
	s_cube->m_indexes.push_back(3);
	s_cube->m_indexes.push_back(1);
	s_cube->m_indexes.push_back(0);
	s_cube->m_indexes.push_back(5);
	s_cube->m_indexes.push_back(4);
}

Cube::~Cube()
{
}
