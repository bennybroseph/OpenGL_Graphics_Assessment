#ifndef _SHAPE3D_H_
#define _SHAPE3D_H_

#include <glm/ext.hpp>
#include <vector>

#include "Shader.h"
#include "Transform.h"

using glm::vec4;

using std::vector;
using std::string;

struct Vertex
{
	vec4 position;
	vec4 colour;
};

class Shape3D
{
public:
	
	virtual void draw();

	static void genBuffers();

	Shader& shader();
	Transform& transform();

	virtual ~Shape3D();

protected:

	static vector<Vertex> s_vertexes;
	static vector<GLuint> s_indexes;

	static GLuint s_VAO;
	static GLuint s_VBO;
	static GLuint s_IBO;

	Shader m_shader = Shader::defaultShader();

	Transform m_transform;

};

#endif // _SHAPE3D_H_