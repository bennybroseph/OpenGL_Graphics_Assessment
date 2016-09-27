#ifndef _MESH_H_
#define _MESH_H_
#pragma once

#include "Texture.h"

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

	vectorPtrS<Vertex> m_vertexes = make_shared<vector<Vertex>>();
	vectorPtrS<GLuint> m_indexes = make_shared<vector<GLuint>>();

	GLuint m_vao = 0;
	GLuint m_vbo = 0;
	GLuint m_ibo = 0;

	TexturePtrS m_texture = make_shared<Texture>();

};

#endif // _MESH_H_