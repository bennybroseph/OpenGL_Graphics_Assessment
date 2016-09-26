#include "Sphere.h"

MeshPtrU Sphere::s_sphere = unique_ptr<Mesh>();

Sphere::Sphere()
{
	m_mesh = s_sphere.get();
	m_drawType = GL_TRIANGLE_STRIP;
}

Sphere::~Sphere() { }

void Sphere::init()
{
	glEnable(GL_PRIMITIVE_RESTART);
	glPrimitiveRestartIndex(0xFFFF);

	auto radius = 1.f;
	auto segments = 30.f;

	s_sphere.reset(new Mesh);

	genVertexes(radius, segments);
	genIndexes(segments, segments);

	s_sphere->genBuffers();
}



void Sphere::genVertexes(const float &radius, const float &segments)
{
	s_sphere->m_vertexes->clear();

	auto halfCircle = genHalfCircle(radius, segments);
	for (auto i = 0.f; i <= segments; ++i)
	{
		auto phi = 2 * PI * (i / segments);
		for (auto vertex : halfCircle)
		{
			vertex.position =
				vec4(
					vertex.position.x * cos(phi) + vertex.position.z * -sin(phi),
					vertex.position.y,
					vertex.position.x * sin(phi) + vertex.position.z * cos(phi), 1.f);
			vertex.normal = vertex.position;
			s_sphere->m_vertexes->push_back(vertex);
		}
	}
}

vector<Vertex> Sphere::genHalfCircle(const float &radius, const float &points)
{
	auto vertexes = vector<Vertex>();
	for (auto i = 0.f; i < points; ++i)
	{
		auto theta = PI * i / (points - 1.f);
		vertexes.push_back(
		{
			vec4(sin(theta) * radius, cos(theta) * radius, 0.f, 1.f),
			vec4(1.f, 1.f, 1.f, 1.f)
		});
	}

	return vertexes;
}

void Sphere::genIndexes(const float &segments, const float &points)
{
	s_sphere->m_indexes->clear();

	//j=np-1
	//
	//2     5   8   11  14  17
	//1     4   7   10  13  16
	//0     3   6   9   12  15
	//
	for (unsigned int i = 0; i < segments; i++) //nm = 4
	{
		unsigned int start = static_cast<unsigned int>(i * points);
		for (auto j = 0; j < points; j++) //np = 3
		{
			unsigned int botR = static_cast<unsigned int>(start + points + j);
			unsigned int botL = start + j;
			s_sphere->m_indexes->push_back(botL);
			s_sphere->m_indexes->push_back(botR);
		}
		s_sphere->m_indexes->push_back(0xFFFF);
	} //we copied the origin whenever we rotated around nm + 1 times so we don't need to get the end again
}

void Sphere::quit()
{
	glDeleteBuffers(1, &s_sphere->m_VBO);
	glDeleteBuffers(1, &s_sphere->m_IBO);

	glDeleteVertexArrays(1, &s_sphere->m_VAO);

	s_sphere.reset();
}