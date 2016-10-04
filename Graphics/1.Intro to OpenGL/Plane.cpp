#include "Plane.h"

namespace Gizmos
{
	const GLfloat Plane::DEFAULT_SEGMENTS = 1.f;

	mapPtrU<string, MeshPtrU> Plane::s_meshes = mapPtrU<string, MeshPtrU>();

	void Plane::init()
	{
		s_meshes.reset(new map <string, MeshPtrU>);

		createMeshIfNeeded();
	}

	ModelPtrU Plane::createModel(float segments)
	{
		auto newModel = make_unique<Model>();

		auto key = createKey(segments);
		createMeshIfNeeded(segments);

		newModel->m_mesh = (*s_meshes)[key].get();
		newModel->m_drawType = GL_TRIANGLES;

		return newModel;
	}

	const Mesh * Plane::getMesh(float segments)
	{
		auto key = createKey(segments);
		createMeshIfNeeded(segments);

		return (*s_meshes)[key].get();
	}

	vectorPtrU<Vertex> Plane::genVertexes(float segments)
	{
		auto vertexes = make_unique<vector<Vertex>>();

		for (auto x = 0.f; x < segments; ++x)
		{
			for (auto y = 0.f; y < segments; ++y)
			{
				vertexes->push_back(
				{
					vec4((x - segments / 2.f) / segments, 0.f, (y - segments / 2.f) / segments, 1.f),
					vec4(1.f, 1.f, 1.f, 1.f),
					vec4(0.f, 1.f, 0.f, 0.f),
					vec4(1.f, 0.f, 0.f, 0.f),
					vec2(x / segments , y / segments),
				});

				vertexes->push_back(
				{
					vec4((x + 1.f - segments / 2.f) / segments, 0.f, (y - segments / 2.f) / segments, 1.f),
					vec4(1.f, 1.f, 1.f, 1.f),
					vec4(0.f, 1.f, 0.f, 0.f),
					vec4(1.f, 0.f, 0.f, 0.f),
					vec2((x + 1.f) / segments, y / segments),
				});

				vertexes->push_back(
				{
					vec4((x + 1.f - segments / 2.f) / segments, 0.f, (y + 1.f - segments / 2.f) / segments, 1.f),
					vec4(1.f, 1.f, 1.f, 1.f),
					vec4(0.f, 1.f, 0.f, 0.f),
					vec4(1.f, 0.f, 0.f, 0.f),
					vec2((x + 1.f) / segments, (y + 1.f) / segments),
				});

				vertexes->push_back(
				{
					vec4((x - segments / 2.f) / segments, 0.f, (y + 1.f - segments / 2.f) / segments, 1.f),
					vec4(1.f, 1.f, 1.f, 1.f),
					vec4(0.f, 1.f, 0.f, 0.f),
					vec4(1.f, 0.f, 0.f, 0.f),
					vec2(x / segments, (y + 1.f) / segments),
				});
			}
		}

		return vertexes;
	}

	vectorPtrU<GLuint> Plane::genIndexes(float segments)
	{
		auto indexes = make_unique<vector<GLuint>>();

		for (auto i = 0; i < segments * segments * 4.f; i += 4)
		{
			indexes->push_back(i);
			indexes->push_back(i + 1);
			indexes->push_back(i + 2);
			indexes->push_back(i);
			indexes->push_back(i + 2);
			indexes->push_back(i + 3);
		}

		/*s_mesh->m_indexes->push_back(0);
		s_mesh->m_indexes->push_back(1);
		s_mesh->m_indexes->push_back(2);
		s_mesh->m_indexes->push_back(0);
		s_mesh->m_indexes->push_back(2);
		s_mesh->m_indexes->push_back(3);*/

		return indexes;
	}

	void Plane::quit()
	{
		s_meshes.reset();
	}

	string Plane::createKey(float segments)
	{
		return to_string(segments);
	}

	bool Plane::createMeshIfNeeded(float segments)
	{
		auto key = createKey(segments);

		auto iter = s_meshes->find(key);
		if (iter == s_meshes->end())
		{
			auto planeMesh = new Mesh();

			planeMesh->m_vertexes = genVertexes(segments);
			planeMesh->m_indexes = genIndexes(segments);
			planeMesh->genBuffers();

			s_meshes->insert_or_assign(key, MeshPtrU(planeMesh));

			return true;
		}

		return false;
	}
}
