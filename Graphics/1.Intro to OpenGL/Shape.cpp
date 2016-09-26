#include "Shape.h"

#include "Camera.h"
#include "Light.h"

void Shape::draw()
{
	if (!m_isEnabled)
		return;

	if (m_mesh != nullptr && m_shouldDrawModel)
	{
		drawModel();
	}

	if (m_shouldDrawWireFrame)
	{
		glLineWidth(1.f);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		{
			auto &shader = *m_shader;
			m_shader = &Shader::standard();

			auto materialColour = *m_materialColour;
			*m_materialColour = vec4(1.f, 1.f, 1.f, 1.f);

			drawModel();

			m_shader = &shader;
			*m_materialColour = materialColour;
		}
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

void Shape::drawModel()
{
	glUseProgram(m_shader->programID());

	unsigned int matUniform = glGetUniformLocation(m_shader->programID(), "ProjectionViewModel");
	glUniformMatrix4fv(
		matUniform,
		1,
		false,
		value_ptr(Camera::mainCamera()->getProjectionView() * m_transform->getWorldSpaceMatrix()));

	// bind the model matrix
	matUniform = glGetUniformLocation(m_shader->programID(), "ModelMatrix");
	glUniformMatrix4fv(matUniform, 1, GL_FALSE, &m_transform->getWorldSpaceMatrix()[0][0]);

	auto normalMatrix = inverse(m_transform->getWorldSpaceMatrix());
	// bind the normal matrix (make it transposed)
	matUniform = glGetUniformLocation(m_shader->programID(), "NormalMatrix");
	glUniformMatrix4fv(matUniform, 1, GL_TRUE, &normalMatrix[0][0]);

	auto lightDirection = (*Light::s_lights)[0]->m_transform->forward();
	// bind light data (not using structs or uniform block for now)
	unsigned int lightUniform = glGetUniformLocation(m_shader->programID(), "LightDirection");
	glUniform3fv(lightUniform, 1, &lightDirection[0]);

	auto lightAmbient = vec4(0.8f);
	// bind ambient light
	lightUniform = glGetUniformLocation(m_shader->programID(), "LightAmbient");
	glUniform3fv(lightUniform, 1, &lightAmbient[0]);

	auto lightDiffuse = *(*Light::s_lights)[0]->m_diffuse;
	lightUniform = glGetUniformLocation(m_shader->programID(), "LightDiffuse");
	glUniform3fv(lightUniform, 1, &lightDiffuse[0]);

	auto lightSpecular = *(*Light::s_lights)[0]->m_specular;
	lightUniform = glGetUniformLocation(m_shader->programID(), "LightSpecular");
	glUniform3fv(lightUniform, 1, &lightSpecular[0]);

	auto materialAmbient = *m_materialColour;
	// bind material
	unsigned int materialUniform = glGetUniformLocation(m_shader->programID(), "MaterialAmbient");
	glUniform3fv(materialUniform, 1, &materialAmbient[0]);

	auto materialDiffuse = *m_materialColour;
	materialUniform = glGetUniformLocation(m_shader->programID(), "MaterialDiffuse");
	glUniform3fv(materialUniform, 1, &materialDiffuse[0]);

	auto materialSpecular = vec3(1.f);
	materialUniform = glGetUniformLocation(m_shader->programID(), "MaterialSpecular");
	glUniform3fv(materialUniform, 1, &materialSpecular[0]);

	// bind camera position for specular calculation
	unsigned int cameraUniform = glGetUniformLocation(m_shader->programID(), "CameraPosition");
	glUniform3fv(cameraUniform, 1, &Camera::mainCamera()->getWorldPosition().getWorldSpaceMatrix()[3][0]);

	auto specularPower = 32.f;
	materialUniform = glGetUniformLocation(m_shader->programID(), "SpecularPower");
	glUniform1f(materialUniform, specularPower);

	unsigned int materialColor = glGetUniformLocation(m_shader->programID(), "vMatColor");
	glUniform4fv(materialColor, 1, value_ptr(*m_materialColour));

	glBindVertexArray(m_mesh->m_VAO);
	glDrawElements(m_drawType, m_mesh->m_indexes->size(), GL_UNSIGNED_INT, nullptr);

	glBindVertexArray(0);
}

const Shader * Shape::getShader() const
{
	return m_shader;
}

void Shape::setShader(const Shader* newShader)
{
	m_shader = newShader;
}

const vec4 & Shape::getMaterialColour() const
{
	return *m_materialColour;
}
// ReSharper disable once CppMemberFunctionMayBeConst
void Shape::setMaterialColour(const vec4& newColour)
{
	*m_materialColour = newColour;
}

Transform * Shape::transform()
{
	return m_transform.get();
}
const Transform * Shape::transform() const
{
	return m_transform.get();
}

GLint * Shape::drawType() const
{
	return &m_drawType;
}

GLboolean * Shape::isEnabled() const
{
	return &m_isEnabled;
}

GLboolean * Shape::shouldDrawModel() const
{
	return &m_shouldDrawModel;
}

GLboolean * Shape::shouldDrawWireFrame() const
{
	return &m_shouldDrawWireFrame;
}

Shape::~Shape() { }
