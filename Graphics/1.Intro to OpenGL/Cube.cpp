#include "Cube.h"

MeshPtrU Cube::s_cube = unique_ptr<Mesh>();

Cube::Cube()
{
	m_mesh = s_cube.get();
	m_drawType = GL_TRIANGLE_STRIP;
}

Cube::~Cube() { }

void Cube::init()
{
	s_cube.reset(new Mesh);

	genVertexes();
	genIndexes();

	s_cube->genBuffers();
}

void Cube::genVertexes()
{
	s_cube->m_vertexes->clear();

	s_cube->m_vertexes->push_back(
	{
		vec4(-0.5f, -0.5f, 0.5f, 1.f),		// Front-Bottom-Left
		vec4(1.f, 1.f, 1.f, 1.f)
	});
	s_cube->m_vertexes->push_back(
	{
		vec4(0.5f, -0.5f, 0.5f, 1.f),		// Front-Bottom-Right
		vec4(1.f, 1.f, 1.f, 1.f)
	});
	s_cube->m_vertexes->push_back(
	{
		vec4(-0.5f, 0.5f, 0.5f, 1.f),		// Front-Top-Left
		vec4(1.f, 1.f, 1.f, 1.f)
	});
	s_cube->m_vertexes->push_back(
	{
		vec4(0.5f, 0.5f, 0.5f, 1.f),		// Front-Top-Right
		vec4(1.f, 1.f, 1.f, 1.f)
	});
	s_cube->m_vertexes->push_back(
	{
		vec4(-0.5f, -0.5f, -0.5f, 1.f),		// Back-Bottom-Left
		vec4(1.f, 1.f, 1.f, 1.f)
	});
	s_cube->m_vertexes->push_back(
	{
		vec4(0.5f, -0.5f, -0.5f, 1.f),		// Back-Bottom-Right
		vec4(1.f, 1.f, 1.f, 1.f)
	});
	s_cube->m_vertexes->push_back(
	{
		vec4(-0.5f, 0.5f, -0.5f, 1.f),		// Back-Top-Left
		vec4(1.f, 1.f, 1.f, 1.f)
	});
	s_cube->m_vertexes->push_back(
	{
		vec4(0.5f, 0.5f, -0.5f, 1.f),		// Back-Top-Right
		vec4(1.f, 1.f, 1.f, 1.f)
	});
}

void Cube::genIndexes()
{
	s_cube->m_indexes->clear();

	s_cube->m_indexes->push_back(7);
	s_cube->m_indexes->push_back(6);
	s_cube->m_indexes->push_back(3);
	s_cube->m_indexes->push_back(2);
	s_cube->m_indexes->push_back(0);
	s_cube->m_indexes->push_back(6);
	s_cube->m_indexes->push_back(4);
	s_cube->m_indexes->push_back(7);
	s_cube->m_indexes->push_back(5);
	s_cube->m_indexes->push_back(3);
	s_cube->m_indexes->push_back(1);
	s_cube->m_indexes->push_back(0);
	s_cube->m_indexes->push_back(5);
	s_cube->m_indexes->push_back(4);
}

void Cube::quit()
{
	glDeleteBuffers(1, &s_cube->m_VBO);
	glDeleteBuffers(1, &s_cube->m_IBO);

	glDeleteVertexArrays(1, &s_cube->m_VAO);

	s_cube.reset();
}