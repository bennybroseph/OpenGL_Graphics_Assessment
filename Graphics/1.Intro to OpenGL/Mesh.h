#ifndef _MESH_H_
#define _MESH_H_

#include <gl_core_4_4.h>
#include <glm/ext.hpp>
#include <vector>

using glm::vec4;

using std::vector;
using std::string;

struct Vertex
{
	vec4 position;
	vec4 colour;
	vec4 normal;
};

class Mesh
{

	friend class Shape;
	friend class Plane;
	friend class Cube;
	friend class Sphere;

public:

	virtual void genBuffers();

	virtual ~Mesh();

protected:

	vector<Vertex> m_vertexes;
	vector<GLuint> m_indexes;

	GLuint m_VAO = 0;
	GLuint m_VBO = 0;
	GLuint m_IBO = 0;

};

#endif // _MESH_H_