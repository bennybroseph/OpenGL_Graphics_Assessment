#include "Cube.h"

namespace Gizmos
{
	vectorPtrU<Vertex> Cube::m_vertexes = unique_ptr<vector<Vertex>>();
	vectorPtrU<GLuint> Cube::m_indexes = unique_ptr<vector<GLuint>>();

	void Cube::init()
	{
		m_vertexes.reset(new vector<Vertex>);
		m_indexes.reset(new vector<GLuint>);

		genVertexes();
		genIndexes();
	}

	ModelPtrU Cube::create()
	{
		auto newModel = make_unique<Model>(nullptr);

		newModel->m_mesh->m_vertexes = m_vertexes.get();
		newModel->m_mesh->m_indexes = m_indexes.get();
		newModel->m_mesh->genBuffers();

		newModel->m_drawType = GL_TRIANGLE_STRIP;

		return newModel;
	}

	void Cube::genVertexes()
	{
		m_vertexes->clear();

		m_vertexes->push_back(
		{
			vec4(-0.5f, -0.5f, 0.5f, 1.f),		// Front-Bottom-Left
			vec4(1.f, 1.f, 1.f, 1.f)
		});
		m_vertexes->push_back(
		{
			vec4(0.5f, -0.5f, 0.5f, 1.f),		// Front-Bottom-Right
			vec4(1.f, 1.f, 1.f, 1.f)
		});
		m_vertexes->push_back(
		{
			vec4(-0.5f, 0.5f, 0.5f, 1.f),		// Front-Top-Left
			vec4(1.f, 1.f, 1.f, 1.f)
		});
		m_vertexes->push_back(
		{
			vec4(0.5f, 0.5f, 0.5f, 1.f),		// Front-Top-Right
			vec4(1.f, 1.f, 1.f, 1.f)
		});
		m_vertexes->push_back(
		{
			vec4(-0.5f, -0.5f, -0.5f, 1.f),		// Back-Bottom-Left
			vec4(1.f, 1.f, 1.f, 1.f)
		});
		m_vertexes->push_back(
		{
			vec4(0.5f, -0.5f, -0.5f, 1.f),		// Back-Bottom-Right
			vec4(1.f, 1.f, 1.f, 1.f)
		});
		m_vertexes->push_back(
		{
			vec4(-0.5f, 0.5f, -0.5f, 1.f),		// Back-Top-Left
			vec4(1.f, 1.f, 1.f, 1.f)
		});
		m_vertexes->push_back(
		{
			vec4(0.5f, 0.5f, -0.5f, 1.f),		// Back-Top-Right
			vec4(1.f, 1.f, 1.f, 1.f)
		});
	}

	void Cube::genIndexes()
	{
		m_indexes->clear();

		m_indexes->push_back(7);
		m_indexes->push_back(6);
		m_indexes->push_back(3);
		m_indexes->push_back(2);
		m_indexes->push_back(0);
		m_indexes->push_back(6);
		m_indexes->push_back(4);
		m_indexes->push_back(7);
		m_indexes->push_back(5);
		m_indexes->push_back(3);
		m_indexes->push_back(1);
		m_indexes->push_back(0);
		m_indexes->push_back(5);
		m_indexes->push_back(4);
	}

	void Cube::quit()
	{
		m_vertexes.reset();
		m_indexes.reset();
	}
}