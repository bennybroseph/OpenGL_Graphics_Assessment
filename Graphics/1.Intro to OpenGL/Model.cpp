#include "Model.h"

#include <imgui_impl_glfw_gl3.h>

#include "Transform.h"
#include "GameObject.h"
#include "Camera.h"
#include "Light.h"
#include <GLFW/glfw3.h>

Model::Model() : Component()
{
	setName("Model");

	m_normalTexture->setName("Normal Map");
	m_diffuseTexture->setName("Diffuse Texture");
	m_specularTexture->setName("Specular Map");

	m_shaderVariables = m_shader->getVariables();

	auto dims = 64.f;
	auto perlin_data = new float[dims * dims];
	auto scale = (1.0f / dims) * 3;
	for (auto x = 0.f; x < dims; ++x)
	{
		for (auto y = 0.f; y < dims; ++y)
		{
			perlin_data[static_cast<int>(y * dims + x)] = perlin(vec2(x, y) * scale) * 0.5f + 0.5f;
		}
	}

	glGenTextures(1, &m_perlin_texture);
	glBindTexture(GL_TEXTURE_2D, m_perlin_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, dims, dims, 0, GL_RED, GL_FLOAT, perlin_data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void Model::draw() const
{
	if (!m_isEnabled || !m_mesh)
		return;

	if (m_shouldDrawModel)
	{
		drawModel(gameObject()->transform()->getWorldSpaceMatrix());
	}
	if (m_shouldDrawWireFrame)
	{
		glLineWidth(1.5f);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		{
			drawModel(gameObject()->transform()->getWorldSpaceMatrix());
		}
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glLineWidth(1.f);
	}
}

void Model::drawGizmos() const
{
	if (!m_mesh)
		return;

	glLineWidth(1.f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	{
		auto &shader = *m_shader;
		m_shader = Shader::standard();

		auto materialColour = *m_materialColour;
		*m_materialColour = vec4(
			1.f - m_materialColour->x * 1.5f,
			1.f - m_materialColour->y * 1.5f,
			1.f - m_materialColour->z * 1.5f, 1.f);

		drawModel(gameObject()->transform()->getWorldSpaceMatrix());

		m_shader = &shader;
		*m_materialColour = materialColour;
	}
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glLineWidth(1.f);
}

void Model::drawGui() const
{
	auto shaderIndex = 0;
	vector<const char *> shaderNames;
	auto i = 0;
	for (auto &shader : *Shader::getShaders())
	{
		shaderNames.push_back(shader->getName());
		if (strcmp(shader->getName(), m_shader->getName()) == 0)
			shaderIndex = i;

		i++;
	}

	if (ImGui::Combo("Shader", &shaderIndex, shaderNames.data(), i))
		setShader((*Shader::getShaders())[shaderIndex]);

	for (auto &variable : *m_shaderVariables)
		variable->drawGui();

	ImGui::LabelText("", "");	// Surely there is a better way, but this works for now

	auto drawTypeIndex = m_drawType - GL_TRIANGLES;
	vector<const char*> drawTypes = { "GL_TRIANGLES", "GL_TRIANGLE_STRIP", "GL_TRIANGLE_FAN" };

	if (ImGui::Combo("Draw Type", &drawTypeIndex, drawTypes.data(), 3))
		m_drawType = drawTypeIndex + GL_TRIANGLES;

	bool shouldDrawModel = m_shouldDrawModel;
	if (ImGui::Checkbox("Draw Model", &shouldDrawModel))
		m_shouldDrawModel = shouldDrawModel;

	bool shouldDrawWireFrame = m_shouldDrawWireFrame;
	ImGui::SameLine();
	if (ImGui::Checkbox("Draw WireFrame", &shouldDrawWireFrame))
		m_shouldDrawWireFrame = shouldDrawWireFrame;

	if (ImGui::CollapsingHeader("Textures", nullptr, false))
	{
		ImGui::Indent();
		{
			for (auto &texture : *m_textures)
				texture->drawGui();

			ImGui::Spacing();
			ImGui::Spacing();

			m_diffuseTexture->drawGui();
			m_normalTexture->drawGui();
			m_specularTexture->drawGui();
		}
		ImGui::Unindent();
	}
}

void Model::drawModel(const mat4 &matrix) const
{
	glUseProgram(m_shader->programID());

	unsigned int matUniform = glGetUniformLocation(m_shader->programID(), "ProjectionViewModel");
	glUniformMatrix4fv(
		matUniform,
		1,
		false,
		value_ptr(Camera::mainCamera()->getProjectionView() * matrix));

	auto time = glGetUniformLocation(m_shader->programID(), "time");
	if (time != -1)
		glUniform1f(time, glfwGetTime());

	// bind the model matrix
	matUniform = glGetUniformLocation(m_shader->programID(), "ModelMatrix");
	if (matUniform != -1)
		glUniformMatrix4fv(matUniform, 1, GL_FALSE, &matrix[0][0]);

	auto normalMatrix = inverse(matrix);
	// bind the normal matrix (make it transposed)
	matUniform = glGetUniformLocation(m_shader->programID(), "NormalMatrix");
	if (matUniform != -1)
		glUniformMatrix4fv(matUniform, 1, GL_TRUE, &normalMatrix[0][0]);

	if (Light::s_lights->size() > 0)
	{
		auto lightDirection = (*Light::s_lights)[0]->gameObject()->transform()->forward();
		// bind light data (not using structs or uniform block for now)
		unsigned int lightUniform = glGetUniformLocation(m_shader->programID(), "LightDirection");
		if (lightUniform != -1)
			glUniform3fv(lightUniform, 1, &lightDirection[0]);

		auto lightAmbient = vec4(0.8f);
		// bind ambient light
		lightUniform = glGetUniformLocation(m_shader->programID(), "LightAmbient");
		if (lightUniform != -1)
			glUniform3fv(lightUniform, 1, &lightAmbient[0]);

		auto lightDiffuse = *(*Light::s_lights)[0]->m_diffuse;
		lightUniform = glGetUniformLocation(m_shader->programID(), "LightDiffuse");
		if (lightUniform != -1)
			glUniform3fv(lightUniform, 1, &lightDiffuse[0]);

		auto lightSpecular = *(*Light::s_lights)[0]->m_specular;
		lightUniform = glGetUniformLocation(m_shader->programID(), "LightSpecular");
		if (lightUniform != -1)
			glUniform3fv(lightUniform, 1, &lightSpecular[0]);
	}

	// bind camera position for specular calculation
	unsigned int cameraUniform = glGetUniformLocation(m_shader->programID(), "CameraPosition");
	if (cameraUniform != -1)
		glUniform3fv(cameraUniform, 1, &Camera::mainCamera()->getWorldPosition().getWorldSpaceMatrix()[3][0]);

	auto textureCount = glGetUniformLocation(m_shader->programID(), "textureCount");
	if (textureCount != -1)
		glUniform1i(textureCount, m_textures->size());

	auto i = 0;
	for (auto &texture : *m_textures)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, texture->getHandle());

		// I could include <string> but...whatever
		char variableName[31] = "textures[";
		char buffer[3];
		_itoa(i, buffer, 3);
		strcat_s(variableName, 31, buffer);
		strcat_s(variableName, 31, "]");

		// tell the shader where it is
		auto location = glGetUniformLocation(m_shader->programID(), variableName);
		if (location != -1)
			glUniform1i(location, i);

		i++;
	}

	if (m_diffuseTexture.get())
	{
		glActiveTexture(GL_TEXTURE29);
		glBindTexture(GL_TEXTURE_2D, m_diffuseTexture->getHandle());

		auto location = glGetUniformLocation(m_shader->programID(), "diffuseMap");
		if (location != -1)
			glUniform1i(location, 29);
	}
	if (m_normalTexture.get())
	{
		glActiveTexture(GL_TEXTURE30);
		glBindTexture(GL_TEXTURE_2D, m_normalTexture->getHandle());

		auto location = glGetUniformLocation(m_shader->programID(), "normalMap");
		if (location != -1)
			glUniform1i(location, 30);
	}

	glActiveTexture(GL_TEXTURE31);
	glBindTexture(GL_TEXTURE_2D, m_perlin_texture);
	auto location = glGetUniformLocation(m_shader->programID(), "perlin_texture");
	if (location != -1)
		glUniform1i(location, 31);

	for (auto &variable : *m_shaderVariables)
		variable->setUniform();

	glBindVertexArray(m_mesh->m_vao);
	glDrawElements(m_drawType, m_mesh->m_indexes->size(), GL_UNSIGNED_INT, nullptr);

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

const Shader * Model::getShader() const
{
	return m_shader;
}
void Model::setShader(const Shader *newShader) const
{
	m_shader = newShader;
	m_shaderVariables = m_shader->getVariables();
}

const vec4 & Model::getMaterialColour() const
{
	return *m_materialColour;
}
// ReSharper disable once CppMemberFunctionMayBeConst
void Model::setMaterialColour(const vec4& newColour)
{
	*m_materialColour = newColour;
}

// ReSharper disable once CppMemberFunctionMayBeConst
int Model::addTexture(const GLchar *path, FilteringType filteringType, const GLchar *name)
{
	auto parsedName = name;
	string tempString;
	if (strcmp(parsedName, "") == 0)
	{
		tempString = "Texture " + to_string(m_textures->size() + 1);
		parsedName = tempString.c_str();
	}

	m_textures->push_back(make_unique<Texture>(path, filteringType, parsedName));

	return 0;
}
// ReSharper disable once CppMemberFunctionMayBeConst
int Model::removeTextureAt(GLuint index)
{
	if (index >= m_textures->size())
		return -1;

	m_textures->erase(m_textures->begin() + index);

	return 0;
}
// ReSharper disable once CppMemberFunctionMayBeConst
int Model::removeTexture(GLuint handle)
{
	auto i = 0;
	for (auto &texture : *m_textures)
	{
		if (texture->getHandle() == handle)
			return removeTextureAt(i);

		i++;
	}

	return -1;
}

int Model::setNormalTexture(const GLchar *path, FilteringType filteringType)
{
	m_normalTexture->setTexture(path);
	m_normalTexture->setFiltering(filteringType);

	return 0;
}
int Model::setDiffuseTexture(const GLchar *path, FilteringType filteringType)
{
	m_diffuseTexture->setTexture(path);
	m_diffuseTexture->setFiltering(filteringType);

	return 0;
}
int Model::setSpecularTexture(const GLchar *path, FilteringType filteringType)
{
	m_specularTexture->setTexture(path);
	m_specularTexture->setFiltering(filteringType);

	return 0;
}

Model::~Model() { }
