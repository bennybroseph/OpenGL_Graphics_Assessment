#include "Plane.h"

Mesh* Plane::s_plane = new Mesh();

Plane::Plane()
{
	m_model = s_plane;
	m_drawType = GL_TRIANGLE_STRIP;
}

void Plane::init()
{
	genVertexes();
	genIndexes();

	s_plane->genBuffers();
}

void Plane::genVertexes()
{
	s_plane->m_vertexes.clear();

	s_plane->m_vertexes.push_back(
	{
		vec4(-1.f, 0.f, -1.f, 1.f),
		vec4(1.f, 1.f, 1.f, 1.f),
		vec4(0.f, 1.f, 0.f, 0.f)
	});
	s_plane->m_vertexes.push_back(
	{
		vec4(-1.f, 0.f, 1.f, 1.f),
		vec4(1.f, 1.f, 1.f, 1.f),
		vec4(0.f, 1.f, 0.f, 0.f)
	});
	s_plane->m_vertexes.push_back(
	{
		vec4(1.f, 0.f, -1.f, 1.f),
		vec4(1.f, 1.f, 1.f, 1.f),
		vec4(0.f, 1.f, 0.f, 0.f),
	});
	s_plane->m_vertexes.push_back(
	{
		vec4(1.f, 0.f, 1.f, 1.f),
		vec4(1.f, 1.f, 1.f, 1.f),
		vec4(0.f, 1.f, 0.f, 0.f)
	});
}

void Plane::genIndexes()
{
	s_plane->m_indexes.clear();

	s_plane->m_indexes.push_back(0);
	s_plane->m_indexes.push_back(1);
	s_plane->m_indexes.push_back(2);
	s_plane->m_indexes.push_back(3);
}

Plane::~Plane()
{
}
