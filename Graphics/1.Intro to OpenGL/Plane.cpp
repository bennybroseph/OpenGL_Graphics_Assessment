#include "Plane.h"

namespace Gizmos
{
	vectorPtrU<Vertex> Plane::m_vertexes = unique_ptr<vector<Vertex>>();
	vectorPtrU<GLuint> Plane::m_indexes = unique_ptr<vector<GLuint>>();

	void Plane::init()
	{
		m_vertexes.reset(new vector<Vertex>);
		m_indexes.reset(new vector<GLuint>);

		genVertexes();
		genIndexes();
	}

	ModelPtrU Plane::create()
	{
		auto newModel = make_unique<Model>();

		newModel->m_mesh->m_vertexes = m_vertexes.get();
		newModel->m_mesh->m_indexes = m_indexes.get();
		newModel->m_mesh->genBuffers();

		newModel->m_drawType = GL_TRIANGLES;

		return newModel;
	}

	void Plane::genVertexes()
	{
		m_vertexes->clear();

		m_vertexes->push_back(
		{
			vec4(-1.f, 0.f, 1.f, 1.f),
			vec4(1.f, 1.f, 1.f, 1.f),
			vec4(0.f, 1.f, 0.f, 0.f),
			vec2(0.f, 1.f),
		});
		m_vertexes->push_back(
		{
			vec4(1.f, 0.f, 1.f, 1.f),
			vec4(1.f, 1.f, 1.f, 1.f),
			vec4(0.f, 1.f, 0.f, 0.f),
			vec2(1.f, 1.f),
		});
		m_vertexes->push_back(
		{
			vec4(1.f, 0.f, -1.f, 1.f),
			vec4(1.f, 1.f, 1.f, 1.f),
			vec4(0.f, 1.f, 0.f, 0.f),
			vec2(1.f, 0.f),
		});
		m_vertexes->push_back(
		{
			vec4(-1.f, 0.f, -1.f, 1.f),
			vec4(1.f, 1.f, 1.f, 1.f),
			vec4(0.f, 1.f, 0.f, 0.f),
			vec2(0.f, 0.f),
		});
	}

	void Plane::genIndexes()
	{
		m_indexes->clear();

		m_indexes->push_back(0);
		m_indexes->push_back(1);
		m_indexes->push_back(2);
		m_indexes->push_back(0);
		m_indexes->push_back(2);
		m_indexes->push_back(3);
	}

	void Plane::quit()
	{
		m_vertexes.reset();
		m_indexes.reset();
	}
}