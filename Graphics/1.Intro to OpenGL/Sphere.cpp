#include "Sphere.h"

namespace Gizmos
{
	mapPtrU<string, MeshPtrU> Sphere::s_meshes = mapPtrU<string, MeshPtrU>();

	void Sphere::init()
	{
		glEnable(GL_PRIMITIVE_RESTART);
		glPrimitiveRestartIndex(0xFFFF);

		s_meshes.reset(new map <string, MeshPtrU>);

		create();
	}

	ModelPtrU Sphere::create(float radius, float segments)
	{
		auto newModel = make_unique<Model>();

		auto key = createKey(radius, segments);
		createMeshIfNeeded(radius, segments);

		newModel->m_mesh = (*s_meshes)[key].get();
		newModel->m_drawType = GL_TRIANGLE_STRIP;

		return newModel;
	}

	const Mesh * Sphere::getMesh(float radius, float segments)
	{
		auto key = createKey(radius, segments);
		createMeshIfNeeded(radius, segments);

		return (*s_meshes)[key].get();
	}

	vectorPtrU<Vertex> Sphere::genVertexes(const float &radius, const float &segments)
	{
		auto vertexes = make_unique<vector<Vertex>>();

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

				vertexes->push_back(vertex);
			}
		}

		return vertexes;
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

	vectorPtrU<GLuint> Sphere::genIndexes(const float &segments, const float &points)
	{
		auto indexes = make_unique<vector<GLuint>>();

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
				indexes->push_back(botL);
				indexes->push_back(botR);
			}
			GLuint restart = 0xFFFF;
			indexes->push_back(restart);
		} //we copied the origin whenever we rotated around nm + 1 times so we don't need to get the end again

		return indexes;
	}

	void Sphere::quit()
	{
		s_meshes.reset();
	}

	string Sphere::createKey(float radius, float segments)
	{
		return to_string(radius) + ' ' + to_string(segments);
	}

	bool Sphere::createMeshIfNeeded(float radius, float segments)
	{
		auto key = createKey(radius, segments);

		auto iter = s_meshes->find(key);
		if (iter == s_meshes->end())
		{
			auto lineMesh = new Mesh();

			lineMesh->m_vertexes = genVertexes(radius, segments);
			lineMesh->m_indexes = genIndexes(segments, segments);
			lineMesh->genBuffers();

			s_meshes->insert_or_assign(key, MeshPtrU(lineMesh));

			return true;
		}

		return false;
	}
}
