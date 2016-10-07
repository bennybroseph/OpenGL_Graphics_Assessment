#include "ShaderVariable.h"

#include <imgui_impl_glfw_gl3.h>

ShaderVariable::ShaderVariable(GLuint shader, const string &text)
{
	m_shader = shader;
	parseType(text);
}

void ShaderVariable::drawGui() const
{
	auto colour = false;
	auto range = false;
	auto rangeData = vec2(0.f, 0.f);
	auto step = 1.f;
	auto min = 0.f;
	auto max = 0.f;
	for (auto &parameter : *m_parameters)
	{
		if (parameter->type == ParameterType::Colour)
			colour = true;

		if (parameter->type == ParameterType::Range)
		{
			range = true;
			rangeData = vec2((*parameter->values)[0], (*parameter->values)[1]);
		}

		if (parameter->type == ParameterType::Min)
			min = (*parameter->values)[0];
		if (parameter->type == ParameterType::Max)
			max = (*parameter->values)[0];

		if (parameter->type == ParameterType::Step)
			step = (*parameter->values)[0];

	}

	switch (m_type)
	{
	case VariableType::Float:
	{
		auto tempFloat = *static_cast<GLfloat *>(m_data.get());
		if (!range)
		{
			if (ImGui::DragFloat(m_displayName.get(), &tempFloat, step, min, max))
				*static_cast<GLfloat *>(m_data.get()) = tempFloat;
		}
		else if (ImGui::SliderFloat(m_displayName.get(), &tempFloat, rangeData[0], rangeData[1]))
			*static_cast<GLfloat *>(m_data.get()) = tempFloat;
	}
	break;

	case VariableType::Vector2:
	{
		auto tempVector = *static_cast<vec2 *>(m_data.get());
		if (!range)
		{
			if (ImGui::DragFloat2(m_displayName.get(), value_ptr(tempVector), step, min, max))
				*static_cast<vec2 *>(m_data.get()) = tempVector;
		}
		else if (ImGui::SliderFloat2(m_displayName.get(), value_ptr(tempVector), rangeData[0], rangeData[1]))
			*static_cast<vec2 *>(m_data.get()) = tempVector;
	}
	break;

	case VariableType::Vector3:
	{
		auto tempVector = *static_cast<vec3 *>(m_data.get());
		if (!colour)
		{
			if (!range)
			{
				if (ImGui::DragFloat3(m_displayName.get(), value_ptr(tempVector), step, min, max))
					*static_cast<vec3 *>(m_data.get()) = tempVector;
			}
			else if (ImGui::SliderFloat3(m_displayName.get(), value_ptr(tempVector), rangeData[0], rangeData[1]))
				*static_cast<vec3 *>(m_data.get()) = tempVector;
		}
		else
		{
			if (ImGui::ColorEdit3(m_displayName.get(), value_ptr(tempVector)))
				*static_cast<vec3 *>(m_data.get()) = tempVector;
		}
	}
	break;

	case VariableType::Vector4:
	{
		auto tempVector = *static_cast<vec4 *>(m_data.get());

		if (!colour)
		{
			if (!range)
			{
				if (ImGui::DragFloat4(m_displayName.get(), value_ptr(tempVector), step, min, max))
					*static_cast<vec4 *>(m_data.get()) = tempVector;
			}
			else if (ImGui::SliderFloat4(m_displayName.get(), value_ptr(tempVector), rangeData[0], rangeData[1]))
				*static_cast<vec4 *>(m_data.get()) = tempVector;
		}
		else
		{
			if (ImGui::ColorEdit4(m_displayName.get(), value_ptr(tempVector)))
				*static_cast<vec4 *>(m_data.get()) = tempVector;
		}
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
	parseParameters(parameters);

	auto currentText = text.substr(text.find("uniform"));

	currentText = currentText.substr(currentText.find(" ") + 1);
	auto type = currentText.substr(0, currentText.find(" "));

	currentText = currentText.substr(currentText.find(" ") + 1);
	parseName(currentText);

	auto equals = currentText.find("=");
	if (type == "float")
	{
		m_type = VariableType::Float;

		auto newFloat = new GLfloat(0.f);
		if (equals != string::npos)
			*newFloat = atof(currentText.substr(equals + 1, currentText.find(";")).c_str());

		m_data.reset(newFloat);
	}

	if (type == "vec2")
	{
		m_type = VariableType::Vector2;

		auto newVector2 = new vec2(0.f);
		if (equals != string::npos)
		{
			int currentFind = currentText.find("(") + 1;

			GLfloat values[2];
			for (auto i = 0; i < 2; ++i)
			{
				if (currentText.find(",") != string::npos)
					values[i] = atof(currentText.substr(currentFind, currentText.find(",")).c_str());
				else
					values[i] = atof(currentText.substr(currentFind, currentText.find(")")).c_str());
				currentFind = currentText.find(",", currentFind) + 1;
			}
			*newVector2 = vec2(values[0], values[1]);
		}

		m_data.reset(newVector2);
	}
	if (type == "vec3")
	{
		m_type = VariableType::Vector3;

		auto newVector3 = new vec3(0.f);
		if (equals != string::npos)
		{
			int currentFind = currentText.find("(") + 1;

			GLfloat values[3];
			for (auto i = 0; i < 3; ++i)
			{
				if (currentText.find(",") != string::npos)
					values[i] = atof(currentText.substr(currentFind, currentText.find(",")).c_str());
				else
					values[i] = atof(currentText.substr(currentFind, currentText.find(")")).c_str());
				currentFind = currentText.find(",", currentFind) + 1;
			}
			*newVector3 = vec3(values[0], values[1], values[2]);
		}
		m_data.reset(newVector3);
	}
	if (type == "vec4")
	{
		m_type = VariableType::Vector4;

		auto newVector4 = new vec4(0.f);
		if (equals != string::npos)
		{
			int currentFind = currentText.find("(") + 1;

			GLfloat values[4];
			for (auto i = 0; i < 4; ++i)
			{
				if (currentText.find(",") != string::npos)
					values[i] = atof(currentText.substr(currentFind, currentText.find(",")).c_str());
				else
					values[i] = atof(currentText.substr(currentFind, currentText.find(")")).c_str());
				currentFind = currentText.find(",", currentFind) + 1;
			}
			*newVector4 = vec4(values[0], values[1], values[2], values[3]);
		}
		m_data.reset(newVector4);

	}

	currentText = currentText.substr(currentText.find(" ") + 1);
}

void ShaderVariable::parseParameters(const string &text)
{
	if (text.find(" ") >= text.find("uniform"))
		return;

	auto parameters = text.substr(text.find(" ") + 1);
	parameters = parameters.substr(0, parameters.find("uniform"));

	auto currentParse = 0;
	while (currentParse != string::npos)
	{
		auto currentParameter = parameters.substr(0, parameters.find(")", currentParse) + 1);
		while (currentParameter.find(" ") != string::npos)
			currentParameter.replace(currentParameter.find(" "), 1, "");

		auto parameterType = currentParameter.substr(0, currentParameter.find("("));

		auto newParameter = make_unique<Parameter>();
		if (parameterType == "Colour" || parameterType == "Color")
			newParameter->type = ParameterType::Colour;

		if (parameterType == "Range")
		{
			newParameter->type = ParameterType::Range;

			auto currentFind = currentParameter.find("(") + 1;
			for (auto i = 0; i < 2; ++i)
			{
				GLfloat value;
				if (currentParameter.find(",") != string::npos)
					value = atof(currentParameter.substr(currentFind, currentParameter.find(",")).c_str());
				else
					value = atof(currentParameter.substr(currentFind, currentParameter.find(")")).c_str());
				currentFind = currentParameter.find(",", currentFind) + 1;

				newParameter->values->push_back(value);
			}
		}

		if (parameterType == "Min" || parameterType == "Max" || parameterType == "Step")
		{
			if (parameterType == "Min")
				newParameter->type = ParameterType::Min;

			if (parameterType == "Max")
				newParameter->type = ParameterType::Max;

			if (parameterType == "Step")
				newParameter->type = ParameterType::Step;

			auto valueParse = currentParameter.substr(currentParameter.find("(") + 1);
			valueParse = valueParse.substr(0, valueParse.find(")"));
			auto value = atof(valueParse.c_str());

			newParameter->values->push_back(value);
		}

		m_parameters->push_back(move(newParameter));

		parameters = parameters.substr(parameters.find(")", currentParse) + 1);
		currentParse = parameters.find(")", currentParse);
	}
}

void ShaderVariable::parseName(const string &text)
{
	string name;
	if (text.find("=") != string::npos)
		name = text.substr(0, text.find("="));
	else
		name = text.substr(0, text.find(";"));
	while (name.find(" ") != string::npos)
		name.replace(name.find(" "), 1, "");

	auto displayName = name;
	if (displayName[0] >= 97 && displayName[0] <= 122)
		displayName[0] -= 32;

	for (auto i = 1; i < displayName.length(); ++i)
	{
		if (displayName[i] >= 65 && displayName[i] <= 90)
		{
			displayName.insert(i, " ");
			i++;
		}
	}

	_memccpy(m_name.get(), name.c_str(), 0, 255);
	_memccpy(m_displayName.get(), displayName.c_str(), 0, 255);
}
