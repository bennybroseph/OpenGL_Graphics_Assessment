#ifndef _MESH_H_
#define _MESH_H_
#pragma once

#include "gl_core_4_4.h"

#include "MasterHeader.h"
#include "Shader.h"

struct Vertex;

typedef unique_ptr<Vertex> VertexPtrU;
typedef shared_ptr<Vertex> VertexPtrS;
typedef weak_ptr<Vertex> VertexPtrW;

struct Vertex
{
	vec4 position;
	vec4 colour;
	vec4 normal;
	vec4 tangent;
	vec2 textureUV;
};

class Mesh;

typedef unique_ptr<Mesh> MeshPtrU;
typedef shared_ptr<Mesh> MeshPtrS;
typedef weak_ptr<Mesh> MeshPtrW;

class Mesh
{
public:

	void drawMesh(
		const mat4 &matrix,
		const vec4 &colour,
		GLboolean drawMesh = 1,
		GLboolean drawWireFrame = 1,
		GLuint drawType = GL_TRIANGLES) const;

	void genBuffers();

	~Mesh();

	vectorPtrU<Vertex> m_vertexes = vectorPtrU<Vertex>();
	vectorPtrU<GLuint> m_indexes = vectorPtrU<GLuint>();

	GLuint m_vao = 0;
	GLuint m_vbo = 0;
	GLuint m_ibo = 0;
};

#endif // _MESH_H_