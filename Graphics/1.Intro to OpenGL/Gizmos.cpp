#include "Gizmos.h"
#include "Camera.h"
#include <glm/detail/type_mat.hpp>
#include <glm/detail/type_mat.hpp>
#include <glm/detail/type_mat.hpp>


namespace Gizmos
{
	int init()
	{
		Plane::init();
		Cube::init();
		Sphere::init();

		return 0;
	}

	int drawSphere(const mat4 &transform, const vec4 &colour, const bool &drawWireFrame)
	{
		auto newSphere = Sphere();

		newSphere.transform().setLocalSpaceMatrix(transform);

		newSphere.materialColour() = colour;

		newSphere.shouldDrawWireFrame() = drawWireFrame;

		newSphere.draw();

		return 0;
	}

	int drawLine(const vec3 & start, const vec3 & end, const vec4 &colour, const GLfloat &width)
	{
		vector<Vertex> vertexes =
		{
			{vec4(start, 1), colour},
			{vec4(end, 1), colour}
		};
		vector<unsigned int> indexes = { 0, 1 };

		GLuint VAO;
		GLuint VBO;
		GLuint IBO;

		glGenBuffers(1, &VBO);
		glGenBuffers(1, &IBO);

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertexes.size() * sizeof(Vertex), vertexes.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexes.size() * sizeof(unsigned int), indexes.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(sizeof(vec4)));

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glUseProgram(Shader::defaultShaderID());
		unsigned int projectionViewUniform = glGetUniformLocation(Shader::defaultShaderID(), "projectionViewWorldMatrix");

		glUniformMatrix4fv(
			projectionViewUniform,
			1,
			false,
			value_ptr(Camera::mainCamera().getProjectionView()));

		unsigned int materialColor = glGetUniformLocation(Shader::defaultShaderID(), "vMatColor");
		glUniform4fv(materialColor, 1, value_ptr(colour));

		glLineWidth(width);
		glBindVertexArray(VAO);
		glDrawElements(GL_LINES, indexes.size(), GL_UNSIGNED_INT, nullptr);

		return 0;
	}

	int quit()
	{
		return 0;
	}
}
