#include "ShaderVariable.h"

#include <imgui_impl_glfw_gl3.h>

ShaderVariable::ShaderVariable(
	GLuint shader,
	void *data,
	const char name[],
	const char displayName[],
	VariableType type)
{
	m_shader = shader;
	m_data.reset(data);
	_memccpy(m_name.get(), name, 0, 255);
	_memccpy(m_displayName.get(), displayName, 0, 255);
	m_type = type;
}


void ShaderVariable::drawGui() const
{
	switch (m_type)
	{
	case VariableType::Float:
	{
		auto tempFloat = *static_cast<GLfloat *>(m_data.get());
		if (ImGui::DragFloat(m_displayName.get(), &tempFloat, 0.25f))
			*static_cast<GLfloat *>(m_data.get()) = tempFloat;
	}
	break;

	case VariableType::Vector2:
	{
		auto tempVector = *static_cast<vec2 *>(m_data.get());
		if (ImGui::DragFloat2(m_displayName.get(), value_ptr(tempVector), 0.25f))
			*static_cast<vec2 *>(m_data.get()) = tempVector;
	}
	break;

	case VariableType::Vector3:
	{
		auto tempVector = *static_cast<vec3 *>(m_data.get());
		if (ImGui::DragFloat3(m_displayName.get(), value_ptr(tempVector), 0.25f))
			*static_cast<vec3 *>(m_data.get()) = tempVector;
	}
	break;

	case VariableType::Vector4:
	{
		auto tempVector = *static_cast<vec4 *>(m_data.get());
		if (ImGui::DragFloat4(m_displayName.get(), value_ptr(tempVector), 0.25f))
			*static_cast<vec4 *>(m_data.get()) = tempVector;
	}
	break;

	default:
		break;
	}
}

void ShaderVariable::setUniform() const
{
	auto location = glGetUniformLocation(m_shader, m_name.get());
	if (location == -1)
		return;

	switch (m_type)
	{
	case VariableType::Float:
	{
		auto tempFloat = static_cast<GLfloat *>(m_data.get());
		glUniform1f(location, *tempFloat);
	}
	break;

	case VariableType::Vector3:
	{
		auto tempVector = static_cast<vec3 *>(m_data.get());
		glUniform3fv(location, 1, value_ptr(*tempVector));
	}
	break;

	case VariableType::Vector4:
	{
		auto tempVector = static_cast<vec4 *>(m_data.get());
		glUniform4fv(location, 1, value_ptr(*tempVector));
	}
	break;

	default:
		break;
	}
}

void ShaderVariable::parseType(const string &text)
{
	auto parameters = text.substr(text.find(" ") + 1);

	int nextSpace = parameters.find(" ");
	int nextUniform = parameters.find("uniform");

	if (nextSpace < nextUniform)
	{
		parameters = parameters.substr(parameters.find(" ") + 1);
		parameters = parameters.substr(0, parameters.find("uniform"));

		auto currentParse = 0;
		while (currentParse != string::npos)
		{
			auto currentParameter = parameters.substr(currentParse, parameters.find(")", currentParse) + 1);

			auto parameterType = currentParameter.substr(0, currentParameter.find("("));

			auto newParameter = new Parameter();
			if (parameterType == "Colour" || parameterType == "Color")
			{
				newParameter->type |= toBit(ParameterType::Colour);
			}
			if (parameterType == "Range")
			{
				newParameter->type |= toBit(ParameterType::Range);
			}
			if (parameterType == "Min")
			{
				newParameter->type |= toBit(ParameterType::Min);
			}
			if (parameterType == "Max")
			{
				newParameter->type |= toBit(ParameterType::Max);
			}

			currentParse = parameters.find(")", currentParse);
		}
	}

	auto currentText = text.substr(text.rfind("uniform"));

	currentText = currentText.substr(currentText.find(" ") + 1);
	auto type = currentText.substr(0, currentText.find(" "));

	if (type == "float")
		m_type = VariableType::Float;

	if (type == "vec2")
		m_type = VariableType::Vector2;
	if (type == "vec3")
		m_type = VariableType::Vector3;
	if (type == "vec4")
		m_type = VariableType::Vector4;

	currentText = currentText.substr(currentText.find(" ") + 1);

}
