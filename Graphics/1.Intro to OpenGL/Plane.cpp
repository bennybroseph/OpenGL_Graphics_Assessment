#include "Plane.h"

MeshPtrU Plane::s_plane = unique_ptr<Mesh>();

Plane::Plane()
{
	m_mesh = s_plane.get();
	m_drawType = GL_TRIANGLE_STRIP;
}

Plane::~Plane() { }

void Plane::init()
{
	s_plane.reset(new Mesh);

	genVertexes();
	genIndexes();

	s_plane->genBuffers();
}

void Plane::genVertexes()
{
	s_plane->m_vertexes->clear();

	s_plane->m_vertexes->push_back(
	{
		vec4(-1.f, 0.f, -1.f, 1.f),
		vec4(1.f, 1.f, 1.f, 1.f),
		vec4(0.f, 1.f, 0.f, 0.f)
	});
	s_plane->m_vertexes->push_back(
	{
		vec4(-1.f, 0.f, 1.f, 1.f),
		vec4(1.f, 1.f, 1.f, 1.f),
		vec4(0.f, 1.f, 0.f, 0.f)
	});
	s_plane->m_vertexes->push_back(
	{
		vec4(1.f, 0.f, -1.f, 1.f),
		vec4(1.f, 1.f, 1.f, 1.f),
		vec4(0.f, 1.f, 0.f, 0.f),
	});
	s_plane->m_vertexes->push_back(
	{
		vec4(1.f, 0.f, 1.f, 1.f),
		vec4(1.f, 1.f, 1.f, 1.f),
		vec4(0.f, 1.f, 0.f, 0.f)
	});
}

void Plane::genIndexes()
{
	s_plane->m_indexes->clear();

	s_plane->m_indexes->push_back(0);
	s_plane->m_indexes->push_back(1);
	s_plane->m_indexes->push_back(2);
	s_plane->m_indexes->push_back(3);
}

void Plane::quit()
{
	glDeleteBuffers(1, &s_plane->m_VBO);
	glDeleteBuffers(1, &s_plane->m_IBO);

	glDeleteVertexArrays(1, &s_plane->m_VAO);

	s_plane.reset();
}