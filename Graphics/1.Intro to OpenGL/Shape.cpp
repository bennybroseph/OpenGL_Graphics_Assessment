#include "Shape.h"

#include "Camera.h"
#include <GLFW/glfw3.h>
#include "Light.h"

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
		glLineWidth(1.f);
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
	m_shader = Shader::phongShader();
	glUseProgram(m_shader.programID());

	unsigned int matUniform = glGetUniformLocation(m_shader.programID(), "ProjectionViewModel");
	glUniformMatrix4fv(
		matUniform,
		1,
		false,
		value_ptr(Camera::mainCamera().getProjectionView() * m_transform.getWorldSpaceMatrix()));

	// bind the model matrix
	matUniform = glGetUniformLocation(m_shader.programID(), "ModelMatrix");
	glUniformMatrix4fv(matUniform, 1, GL_FALSE, &m_transform.getWorldSpaceMatrix()[0][0]);

	auto normalMatrix = inverse(m_transform.getWorldSpaceMatrix());
	// bind the normal matrix (make it transposed)
	matUniform = glGetUniformLocation(m_shader.programID(), "NormalMatrix");
	glUniformMatrix4fv(matUniform, 1, GL_TRUE, &normalMatrix[0][0]);

	auto time = glfwGetTime() * 0.1f;
	auto lightDirection =
		(Light::s_lights[0]->m_transform.getWorldSpaceMatrix()
		- Light::s_lights[0]->m_transform.getWorldSpaceMatrix() * translate(vec3(1.f, 0.f, 0.f)))[3];
	// bind light data (not using structs or uniform block for now)
	unsigned int lightUniform = glGetUniformLocation(m_shader.programID(), "LightDirection");
	glUniform3fv(lightUniform, 1, &lightDirection[0]);

	auto lightAmbient = vec4(0.25f);
	// bind ambient light
	lightUniform = glGetUniformLocation(m_shader.programID(), "LightAmbient");
	glUniform3fv(lightUniform, 1, &lightAmbient[0]);

	auto lightDiffuse = vec4(1.f);
	lightUniform = glGetUniformLocation(m_shader.programID(), "LightDiffuse");
	glUniform3fv(lightUniform, 1, &lightDiffuse[0]);

	auto lightSpecular = vec4(1.f);
	lightUniform = glGetUniformLocation(m_shader.programID(), "LightSpecular");
	glUniform3fv(lightUniform, 1, &lightSpecular[0]);

	auto materialAmbient = m_materialColour;
	// bind material
	unsigned int materialUniform = glGetUniformLocation(m_shader.programID(), "MaterialAmbient");
	glUniform3fv(materialUniform, 1, &materialAmbient[0]);

	auto materialDiffuse = m_materialColour;
	materialUniform = glGetUniformLocation(m_shader.programID(), "MaterialDiffuse");
	glUniform3fv(materialUniform, 1, &materialDiffuse[0]);

	auto materialSpecular = vec4(1.f);
	materialUniform = glGetUniformLocation(m_shader.programID(), "MaterialSpecular");
	glUniform3fv(materialUniform, 1, &materialSpecular[0]);

	// bind camera position for specular calculation
	unsigned int cameraUniform = glGetUniformLocation(m_shader.programID(), "CameraPosition");
	glUniform3fv(cameraUniform, 1, &Camera::mainCamera().getWorldPosition().getWorldSpaceMatrix()[3][0]);

	auto specularPower = 64.f;
	materialUniform = glGetUniformLocation(m_shader.programID(), "SpecularPower");
	glUniform1f(materialUniform, specularPower);

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
