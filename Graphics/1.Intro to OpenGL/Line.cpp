#include "Line.h"



namespace Gizmos
{
	mapPtrU<string, MeshPtrU> Line::s_meshes = mapPtrU<string, MeshPtrU>();

	void Line::init()
	{
		s_meshes.reset(new map<string, MeshPtrU>);
	}

	ModelPtrU Line::create(
		const vec3 &start,
		const vec3 &end,
		const vec4 &colourStart,
		const vec4 &colourEnd)
	{
		auto newModel = make_unique<Model>();

		auto key = createKey(start, end, colourStart, colourEnd);

		createMeshIfNeeded(start, end, colourStart, colourEnd);

		newModel->m_mesh = (*s_meshes)[key].get();
		newModel->m_drawType = GL_LINES;

		return newModel;
	}

	const Mesh* Line::getMesh(const vec3& start, const vec3& end, const vec4& colourStart, const vec4& colourEnd)
	{
		auto key = createKey(start, end, colourStart, colourEnd);
		createMeshIfNeeded(start, end, colourStart, colourEnd);

		return (*s_meshes)[key].get();
	}

	void Line::quit()
	{
		s_meshes.reset();
	}

	string Line::createKey(const vec3& start, const vec3& end, const vec4& colourStart, const vec4& colourEnd)
	{
		return
			'(' + to_string(start.x) + ", " + to_string(start.y) + ", " + to_string(start.z) + ')' +
			'(' + to_string(end.x) + ", " + to_string(end.y) + ", " + to_string(end.z) + ')' +
			'(' + to_string(colourStart.r) + ", " + to_string(colourStart.g) + ", "
			+ to_string(colourStart.b) + ", " + to_string(colourStart.a) + ')' +
			'(' + to_string(colourEnd.r) + ", " + to_string(colourEnd.g) + ", "
			+ to_string(colourEnd.b) + ", " + to_string(colourEnd.a) + ')';
	}
	bool Line::createMeshIfNeeded(
		const vec3& start,
		const vec3& end,
		const vec4& colourStart,
		const vec4& colourEnd)
	{
		auto key = createKey(start, end, colourStart, colourEnd);

		auto iter = s_meshes->find(key);
		if (iter == s_meshes->end())
		{
			auto lineMesh = new Mesh();

			lineMesh->m_vertexes = genVertexes(start, end, colourStart, colourEnd);
			lineMesh->m_indexes = genIndexes();
			lineMesh->genBuffers();

			s_meshes->insert_or_assign(key, MeshPtrU(lineMesh));

			return true;
		}

		return false;
	}

	vectorPtrU<Vertex>  Line::genVertexes(
		const vec3 &start,
		const vec3 &end,
		const vec4 &colourStart,
		const vec4 &colourEnd)
	{
		auto vertexes = make_unique<vector<Vertex>>();

		vertexes->push_back(
		{
			vec4(start.x, start.y, start.z, 1.f),
			vec4(colourStart.r, colourStart.g, colourStart.b, colourStart.a),
			vec4(0.f, 1.f, 0.f, 0.f),
			vec4(1.f, 0.f, 0.f, 0.f),
			vec2(0.f, 1.f),
		});
		vertexes->push_back(
		{
			vec4(end.x, end.y, end.z, 1.f),
			vec4(colourEnd.r, colourEnd.g, colourEnd.b, colourEnd.a),
			vec4(0.f, 1.f, 0.f, 0.f),
			vec4(1.f, 0.f, 0.f, 0.f),
			vec2(1.f, 1.f),
		});

		return vertexes;
	}

	vectorPtrU<GLuint> Line::genIndexes()
	{
		auto indexes = make_unique<vector<GLuint>>();

		indexes->push_back(0);
		indexes->push_back(1);

		return indexes;
	}
}