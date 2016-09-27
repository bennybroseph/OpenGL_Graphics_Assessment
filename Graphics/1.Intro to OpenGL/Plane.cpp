#include "Plane.h"

MeshPtrS Plane::s_plane = shared_ptr<Mesh>();

Plane::Plane()
{
	m_model->m_mesh = *s_plane;
	m_model->m_drawType = GL_TRIANGLES;
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
		vec4(0.f, 1.f, 0.f, 0.f),
		vec2(0.f, 1.f),
	});
	s_plane->m_vertexes->push_back(
	{
		vec4(-1.f, 0.f, 1.f, 1.f),
		vec4(1.f, 1.f, 1.f, 1.f),
		vec4(0.f, 1.f, 0.f, 0.f),
		vec2(1.f, 1.f),
	});
	s_plane->m_vertexes->push_back(
	{
		vec4(1.f, 0.f, -1.f, 1.f),
		vec4(1.f, 1.f, 1.f, 1.f),
		vec4(0.f, 1.f, 0.f, 0.f),
		vec2(1.f, 1.f),
	});
	s_plane->m_vertexes->push_back(
	{
		vec4(1.f, 0.f, 1.f, 1.f),
		vec4(1.f, 1.f, 1.f, 1.f),
		vec4(0.f, 1.f, 0.f, 0.f),
		vec2(1.f, 1.f),
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
	glDeleteBuffers(1, &s_plane->m_vbo);
	glDeleteBuffers(1, &s_plane->m_ibo);

	glDeleteVertexArrays(1, &s_plane->m_vao);

	s_plane.reset();
}