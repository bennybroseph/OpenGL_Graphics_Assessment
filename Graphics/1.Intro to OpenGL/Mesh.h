#ifndef _MESH_H_
#define _MESH_H_
#pragma once

#include <gl_core_4_4.h>

#include "MasterHeader.h"

struct Vertex;

typedef unique_ptr<Vertex> VertexPtrU;
typedef shared_ptr<Vertex> VertexPtrS;
typedef weak_ptr<Vertex> VertexPtrW;

struct Vertex
{
	vec4 position;
	vec4 colour;
	vec4 normal;
};

class Mesh;

typedef unique_ptr<Mesh> MeshPtrU;
typedef shared_ptr<Mesh> MeshPtrS;
typedef weak_ptr<Mesh> MeshPtrW;

class Mesh
{

	friend class Shape;
	friend class Plane;
	friend class Cube;
	friend class Sphere;

public:

	~Mesh();

private:

	void genBuffers();

	const vectorPtrU<Vertex> m_vertexes = make_unique<vector<Vertex>>();
	const vectorPtrU<GLuint> m_indexes = make_unique<vector<GLuint>>();

	GLuint m_VAO = 0;
	GLuint m_VBO = 0;
	GLuint m_IBO = 0;

};

#endif // _MESH_H_