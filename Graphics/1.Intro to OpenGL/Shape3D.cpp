#include "Shape3D.h"

#include "Camera.h"

using std::fstream;

using glm::mat4;
using glm::vec3;

vector<Vertex> Shape3D::s_vertexes = vector<Vertex>();
vector<GLuint> Shape3D::s_indexes = vector<GLuint>();

GLuint Shape3D::s_VAO = 0;
GLuint Shape3D::s_VBO = 0;
GLuint Shape3D::s_IBO = 0;

void Shape3D::genBuffers()
{
	//gen vbo,ibo
	glGenBuffers(1, &s_VBO);
	glGenBuffers(1, &s_IBO);

	glGenVertexArrays(1, &s_VAO);
	glBindVertexArray(s_VAO);

	//bind and pop vbo
	glBindBuffer(GL_ARRAY_BUFFER, s_VBO);
	glBufferData(GL_ARRAY_BUFFER, s_vertexes.size() * sizeof(Vertex), s_vertexes.data(), GL_STATIC_DRAW);

	//bind and pop ibo
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, s_indexes.size() * sizeof(unsigned int), s_indexes.data(), GL_STATIC_DRAW);

	//setup vertex descriptors
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(sizeof(vec4)));

	//teardown unbind all handles
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Shape3D::draw()
{
	glUseProgram(m_shader.programID());
	unsigned int projectionViewUniform = glGetUniformLocation(m_shader.programID(), "projectionViewWorldMatrix");

	glUniformMatrix4fv(
		projectionViewUniform,
		1,
		false,
		value_ptr(Camera::mainCamera().getProjectionView() * transform().getWorldSpaceMatrix()));

	glBindVertexArray(s_VAO);
	glDrawElements(GL_TRIANGLE_STRIP, s_indexes.size(), GL_UNSIGNED_INT, nullptr);
}

Shader& Shape3D::shader()
{
	return m_shader;
}

Transform & Shape3D::transform()
{
	return m_transform;
}

Shape3D::~Shape3D() { }
