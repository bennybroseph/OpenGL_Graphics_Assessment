#include "Sphere.h"

namespace Gizmos
{
	vectorPtrU<Vertex> Sphere::m_vertexes = unique_ptr<vector<Vertex>>();
	vectorPtrU<GLuint> Sphere::m_indexes = unique_ptr<vector<GLuint>>();

	void Sphere::init()
	{
		glEnable(GL_PRIMITIVE_RESTART);
		glPrimitiveRestartIndex(0xFFFF);

		auto radius = 0.5f;
		auto segments = 20.f;

		m_vertexes.reset(new vector<Vertex>);
		m_indexes.reset(new vector<GLuint>);

		genVertexes(radius, segments);
		genIndexes(segments, segments);
	}

	ModelPtrU Sphere::create()
	{
		auto newModel = make_unique<Model>();

		newModel->m_mesh->m_vertexes = m_vertexes.get();
		newModel->m_mesh->m_indexes = m_indexes.get();
		newModel->m_mesh->genBuffers();

		newModel->m_drawType = GL_TRIANGLE_STRIP;

		return newModel;
	}

	void Sphere::genVertexes(const float &radius, const float &segments)
	{
		m_vertexes->clear();

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

				// Calculate uvs
				auto d = normalize(vertex.position);
				vertex.textureUV.x = 0.5f + atan2(d.z, d.x) / (2 * PI);
				vertex.textureUV.y = 0.5f - asin(d.y) / PI;

				m_vertexes->push_back(vertex);
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
				vec4(1.f, 1.f, 1.f, 1.f),
			});
		}

		return vertexes;
	}

	void Sphere::genIndexes(const float &segments, const float &points)
	{
		m_indexes->clear();

		//j=np-1
		//
		//2     5   8   11  14  17
		//1     4   7   10  13  16
		//0     3   6   9   12  15

		for (unsigned int i = 0; i < segments; i++) //nm = 4
		{
			unsigned int start = static_cast<unsigned int>(i * points);
			for (auto j = 0; j < points; j++) //np = 3
			{
				auto botR = static_cast<GLuint>(start + points + j);
				auto botL = start + j;
				m_indexes->push_back(botL);
				m_indexes->push_back(botR);
			}
			GLuint restart = 0xFFFF;
			m_indexes->push_back(restart);
		} //we copied the origin whenever we rotated around nm + 1 times so we don't need to get the end again
	}

	void Sphere::quit()
	{
		m_vertexes.reset();
		m_indexes.reset();
	}
}