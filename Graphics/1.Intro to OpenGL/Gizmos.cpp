#include "Gizmos.h"

#include "Camera.h"

namespace Gizmos
{
	int init()
	{
		Plane::init();
		Cube::init();
		Sphere::init();

		return 0;
	}

	int drawSphere(const mat4 &transform, const vec4 &colour, GLboolean drawWireFrame)
	{
		auto newSphere = new Sphere();

		newSphere->transform()->setLocalSpaceMatrix(transform);

		newSphere->setMaterialColour(colour);

		*newSphere->shouldDrawWireFrame() = drawWireFrame;

		newSphere->draw();

		delete newSphere;

		return 0;
	}
	// TODO: This function is so expensive and needs to be rewritten...please only buffer once somehow
	int drawLine(const vec3 & start, const vec3 & end, const vec4 &colour, GLfloat width)
	{
		vector<Vertex> vertexes =
		{
			{vec4(start, 1), colour},
			{vec4(end, 1), colour}
		};
		vector<GLuint> indexes = { 0, 1 };

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
		glBufferData(
			GL_ELEMENT_ARRAY_BUFFER,
			indexes.size() * sizeof(GLuint),
			indexes.data(),
			GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(sizeof(vec4)));

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glUseProgram(Shader::defaultShaderID());
		GLuint projectionViewUniform =
			glGetUniformLocation(Shader::defaultShaderID(), "ProjectionViewModel");

		glUniformMatrix4fv(
			projectionViewUniform,
			1,
			false,
			value_ptr(Camera::mainCamera()->getProjectionView()));

		GLuint materialColor = glGetUniformLocation(Shader::defaultShaderID(), "vMatColor");
		glUniform4fv(materialColor, 1, value_ptr(colour));

		glBindVertexArray(VAO);

		glLineWidth(width);
		{
			glDrawElements(GL_LINES, indexes.size(), GL_UNSIGNED_INT, nullptr);
		}
		glLineWidth(1.f);

		// Don't forget to delete the data
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &IBO);

		glDeleteVertexArrays(1, &VAO);

		return 0;
	}

	int drawGrid(const vec3 &center, const vec2 &spacing, const vec2 &segments, GLfloat lineWidth)
	{
		struct Line
		{
			vec3 start, end;
			vec4 colour;
		};

		auto lines = vector<Line>();

		auto originColour = vec4(1);
		auto otherColour = vec4(0.75f, 0.75f, 0.75f, 1.f);

		for (auto i = 0; i <= segments.y; ++i)
		{
			lines.push_back(
			{
				vec3(center.x - spacing.x * segments.x, 0.f, i * spacing.y),
				vec3(center.x + spacing.x * segments.x, 0.f, i * spacing.y),
				i == 0 ? originColour : otherColour
			});
			if (i != 0)
			{
				lines.push_back(
				{
					vec3(center.x - spacing.x * segments.x, 0.f, -i * spacing.y),
					vec3(center.x + spacing.x * segments.x, 0.f, -i * spacing.y),
					otherColour
				});
			}
		}
		for (auto i = 0; i <= segments.x; ++i)
		{
			lines.push_back(
			{
				vec3(i * spacing.x, 0.f, center.y - spacing.y * segments.y),
				vec3(i * spacing.x, 0.f, center.y + spacing.y * segments.y),
				i == 0 ? originColour : otherColour
			});
			if (i != 0)
			{
				lines.push_back(
				{
					vec3(-i * spacing.x, 0.f, center.y - spacing.y * segments.y),
					vec3(-i * spacing.x, 0.f, center.y + spacing.y * segments.y),
					otherColour
				});
			}
		}

		for (auto line : lines)
			Gizmos::drawLine(line.start, line.end, line.colour);

		return 0;
	}

	int quit()
	{
		Plane::quit();
		Cube::quit();
		Sphere::quit();


		return 0;
	}
}
