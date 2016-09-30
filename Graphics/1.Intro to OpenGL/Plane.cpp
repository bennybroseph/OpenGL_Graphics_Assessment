#include "Plane.h"

namespace Gizmos
{
	MeshPtrU Plane::s_mesh = MeshPtrU();

	void Plane::init()
	{
		s_mesh.reset(new Mesh);

		genVertexes();
		genIndexes();

		s_mesh->genBuffers();
	}

	ModelPtrU Plane::createModel()
	{
		auto newModel = make_unique<Model>();

		newModel->m_mesh = s_mesh.get();
		newModel->m_drawType = GL_TRIANGLES;

		return newModel;
	}

	const Mesh * Plane::getMesh()
	{
		return s_mesh.get();
	}

	void Plane::genVertexes()
	{
		s_mesh->m_vertexes.reset(new vector<Vertex>);

		s_mesh->m_vertexes->push_back(
		{
			vec4(-1.f, 0.f, 1.f, 1.f),
			vec4(1.f, 1.f, 1.f, 1.f),
			vec4(0.f, 1.f, 0.f, 0.f),
			vec4(1.f, 0.f, 0.f, 0.f),
			vec2(0.f, 1.f),
		});
		s_mesh->m_vertexes->push_back(
		{
			vec4(1.f, 0.f, 1.f, 1.f),
			vec4(1.f, 1.f, 1.f, 1.f),
			vec4(0.f, 1.f, 0.f, 0.f),
			vec4(1.f, 0.f, 0.f, 0.f),
			vec2(1.f, 1.f),
		});
		s_mesh->m_vertexes->push_back(
		{
			vec4(1.f, 0.f, -1.f, 1.f),
			vec4(1.f, 1.f, 1.f, 1.f),
			vec4(0.f, 1.f, 0.f, 0.f),
			vec4(1.f, 0.f, 0.f, 0.f),
			vec2(1.f, 0.f),
		});
		s_mesh->m_vertexes->push_back(
		{
			vec4(-1.f, 0.f, -1.f, 1.f),
			vec4(1.f, 1.f, 1.f, 1.f),
			vec4(0.f, 1.f, 0.f, 0.f),
			vec4(1.f, 0.f, 0.f, 0.f),
			vec2(0.f, 0.f),
		});
	}

	void Plane::genIndexes()
	{
		s_mesh->m_indexes.reset(new vector<GLuint>);

		s_mesh->m_indexes->push_back(0);
		s_mesh->m_indexes->push_back(1);
		s_mesh->m_indexes->push_back(2);
		s_mesh->m_indexes->push_back(0);
		s_mesh->m_indexes->push_back(2);
		s_mesh->m_indexes->push_back(3);
	}

	void Plane::quit()
	{
		s_mesh.reset();
	}
}