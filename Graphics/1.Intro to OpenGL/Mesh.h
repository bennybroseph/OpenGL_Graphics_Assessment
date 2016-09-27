#ifndef _MESH_H_
#define _MESH_H_
#pragma once

#include "gl_core_4_4.h"

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
	vec2 textureUV;
};

class Mesh;

typedef unique_ptr<Mesh> MeshPtrU;
typedef shared_ptr<Mesh> MeshPtrS;
typedef weak_ptr<Mesh> MeshPtrW;

class Mesh
{
public:

	void genBuffers();

	~Mesh();

	const vector<Vertex> *m_vertexes = nullptr;
	const vector<GLuint> *m_indexes = nullptr;

	GLuint m_vao = 0;
	GLuint m_vbo = 0;
	GLuint m_ibo = 0;
};

#endif // _MESH_H_