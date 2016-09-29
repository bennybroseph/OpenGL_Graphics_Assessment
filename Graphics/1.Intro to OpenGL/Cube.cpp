#include "Cube.h"

namespace Gizmos
{
	MeshPtrU Cube::s_mesh = MeshPtrU();

	void Cube::init()
	{
		s_mesh.reset(new Mesh);

		genVertexes();
		genIndexes();

		s_mesh->genBuffers();
	}

	ModelPtrU Cube::create()
	{
		auto newModel = make_unique<Model>();

		newModel->m_mesh = s_mesh.get();
		newModel->m_drawType = GL_TRIANGLE_STRIP;

		return newModel;
	}

	void Cube::genVertexes()
	{
		s_mesh->m_vertexes.reset(new vector<Vertex>);

		s_mesh->m_vertexes->push_back(
		{
			vec4(-0.5f, -0.5f, 0.5f, 1.f),		// Front-Bottom-Left
			vec4(1.f, 1.f, 1.f, 1.f)
		});
		s_mesh->m_vertexes->push_back(
		{
			vec4(0.5f, -0.5f, 0.5f, 1.f),		// Front-Bottom-Right
			vec4(1.f, 1.f, 1.f, 1.f)
		});
		s_mesh->m_vertexes->push_back(
		{
			vec4(-0.5f, 0.5f, 0.5f, 1.f),		// Front-Top-Left
			vec4(1.f, 1.f, 1.f, 1.f)
		});
		s_mesh->m_vertexes->push_back(
		{
			vec4(0.5f, 0.5f, 0.5f, 1.f),		// Front-Top-Right
			vec4(1.f, 1.f, 1.f, 1.f)
		});
		s_mesh->m_vertexes->push_back(
		{
			vec4(-0.5f, -0.5f, -0.5f, 1.f),		// Back-Bottom-Left
			vec4(1.f, 1.f, 1.f, 1.f)
		});
		s_mesh->m_vertexes->push_back(
		{
			vec4(0.5f, -0.5f, -0.5f, 1.f),		// Back-Bottom-Right
			vec4(1.f, 1.f, 1.f, 1.f)
		});
		s_mesh->m_vertexes->push_back(
		{
			vec4(-0.5f, 0.5f, -0.5f, 1.f),		// Back-Top-Left
			vec4(1.f, 1.f, 1.f, 1.f)
		});
		s_mesh->m_vertexes->push_back(
		{
			vec4(0.5f, 0.5f, -0.5f, 1.f),		// Back-Top-Right
			vec4(1.f, 1.f, 1.f, 1.f)
		});
	}

	void Cube::genIndexes()
	{
		s_mesh->m_indexes.reset(new vector<GLuint>);

		s_mesh->m_indexes->push_back(7);
		s_mesh->m_indexes->push_back(6);
		s_mesh->m_indexes->push_back(3);
		s_mesh->m_indexes->push_back(2);
		s_mesh->m_indexes->push_back(0);
		s_mesh->m_indexes->push_back(6);
		s_mesh->m_indexes->push_back(4);
		s_mesh->m_indexes->push_back(7);
		s_mesh->m_indexes->push_back(5);
		s_mesh->m_indexes->push_back(3);
		s_mesh->m_indexes->push_back(1);
		s_mesh->m_indexes->push_back(0);
		s_mesh->m_indexes->push_back(5);
		s_mesh->m_indexes->push_back(4);
	}

	void Cube::quit()
	{
		s_mesh.reset();
	}
}