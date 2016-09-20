#include "Shape.h"

#include "Camera.h"

void Shape::draw()
{
	if (!m_isEnabled)
		return;

	if (m_model != nullptr && m_shouldDrawModel)
	{
		drawModel();
	}

	if (m_shouldDrawWireFrame)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		{
			auto shader = m_shader;
			m_shader = Shader::defaultShader();

			auto materialColour = m_materialColour;
			m_materialColour = vec4(1.f, 1.f, 1.f, 1.f);

			drawModel();

			m_shader = shader;
			m_materialColour = materialColour;
		}
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

void Shape::drawModel()
{
	glUseProgram(m_shader.programID());
	unsigned int projectionViewUniform = glGetUniformLocation(m_shader.programID(), "projectionViewWorldMatrix");

	glUniformMatrix4fv(
		projectionViewUniform,
		1,
		false,
		value_ptr(Camera::mainCamera().getProjectionView() * transform().getWorldSpaceMatrix()));

	unsigned int materialColor = glGetUniformLocation(m_shader.programID(), "vMatColor");
	glUniform4fv(materialColor, 1, value_ptr(m_materialColour));

	glBindVertexArray(m_model->m_VAO);
	glDrawElements(m_drawType, m_model->m_indexes.size(), GL_UNSIGNED_INT, nullptr);
}

GLint& Shape::drawType()
{
	return m_drawType;
}

Shader& Shape::shader()
{
	return m_shader;
}

vec4 & Shape::materialColour()
{
	return m_materialColour;
}

Transform& Shape::transform()
{
	return m_transform;
}

bool& Shape::isEnabled()
{
	return m_isEnabled;
}

bool& Shape::shouldDrawModel()
{
	return m_shouldDrawModel;
}

bool& Shape::shouldDrawWireFrame()
{
	return m_shouldDrawWireFrame;
}

Shape::~Shape() { }
