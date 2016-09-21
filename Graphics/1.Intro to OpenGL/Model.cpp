#include "Model.h"

#include "Camera.h"

using std::fstream;

using glm::mat4;
using glm::vec3;

void Model::genBuffers()
{
	//gen VBO, IBO
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_IBO);

	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	//bind and pop VBO
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, m_vertexes.size() * sizeof(Vertex), m_vertexes.data(), GL_STATIC_DRAW);

	//bind and pop IBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indexes.size() * sizeof(unsigned int), m_indexes.data(), GL_STATIC_DRAW);

	//setup vertex descriptors
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(sizeof(vec4)));

	// normals
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex), reinterpret_cast<void*>(sizeof(vec4) * 2));

	//teardown unbind all handles
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

Model::~Model() { }
