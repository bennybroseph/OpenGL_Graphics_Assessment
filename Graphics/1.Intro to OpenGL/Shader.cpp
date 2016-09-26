#include "Shader.h"

#include <fstream>
#include <string>

using std::fstream;
using std::string;

ShaderPtrU Shader::s_default = unique_ptr<Shader>();
ShaderPtrU Shader::s_positional = unique_ptr<Shader>();
ShaderPtrU Shader::s_phong = unique_ptr<Shader>();

int Shader::init()
{
	s_default.reset(new Shader());
	s_default->m_programID = glCreateProgram();

	auto returnValue = s_default->addShader("Default.vert", ShaderType::Vertex);
	if (returnValue != 0)
		return returnValue;
	returnValue = s_default->addShader("Default.frag", ShaderType::Fragment);
	if (returnValue != 0)
		return returnValue;

	s_positional.reset(new Shader());
	s_positional->m_programID = glCreateProgram();

	returnValue = s_positional->addShader("Positional.vert", ShaderType::Vertex);
	if (returnValue != 0)
		return returnValue;
	returnValue = s_positional->addShader("Default.frag", ShaderType::Fragment);
	if (returnValue != 0)
		return returnValue;

	s_phong.reset(new Shader());
	s_phong->m_programID = glCreateProgram();

	returnValue = s_phong->addShader("Default.vert", ShaderType::Vertex);
	if (returnValue != 0)
		return returnValue;
	returnValue = s_phong->addShader("Phong.frag", ShaderType::Fragment);
	if (returnValue != 0)
		return returnValue;

	return returnValue;
}

int Shader::quit()
{
	s_default.reset();
	s_positional.reset();
	s_phong.reset();

	return 0;
}

int Shader::addShader(const char *path, ShaderType type) const
{
	GLint parsedTypeInt;
	const char* parsedTypeName;
	switch (type)
	{
	case ShaderType::Vertex:
		parsedTypeInt = GL_VERTEX_SHADER;
		parsedTypeName = "vertex";
		break;
	case ShaderType::Fragment:
		parsedTypeInt = GL_FRAGMENT_SHADER;
		parsedTypeName = "fragment";
		break;
	case ShaderType::Geometry:
		parsedTypeInt = GL_GEOMETRY_SHADER;
		parsedTypeName = "geometry";
		break;

	default:
		return -1;
	}

	fstream shaderFile;
	shaderFile.open(path, std::ios::in);

	string parsedText = "";
	string line;
	while (getline(shaderFile, line))
		parsedText += line + "\n";

	shaderFile.close();

	auto vertexShaderSource = parsedText.c_str();

	auto shader = glCreateShader(parsedTypeInt);
	glShaderSource(shader, 1, &vertexShaderSource, nullptr);
	glCompileShader(shader);

	auto success = GL_FALSE;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE)
	{
		auto infoLogLength = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
		auto infoLog = new char[infoLogLength];

		glGetShaderInfoLog(shader, infoLogLength, &infoLogLength, infoLog);
		printf(
			"Error: Failed to compile %s getShader at path %s!\n",
			parsedTypeName,
			path);
		printf("%s\n", infoLog);
		delete[] infoLog;

		return -1;
	}

	glAttachShader(m_programID, shader);
	glLinkProgram(m_programID);

	success = GL_FALSE;
	glGetProgramiv(m_programID, GL_LINK_STATUS, &success);
	if (success == GL_FALSE)
	{
		auto infoLogLength = 0;
		glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &infoLogLength);
		auto infoLog = new char[infoLogLength];

		glGetProgramInfoLog(m_programID, infoLogLength, nullptr, infoLog);
		printf("Error: Failed to link getShader program!\n");
		printf("%s\n", infoLog);
		delete[] infoLog;

		return -1;
	}

	glDeleteShader(shader);

	return 0;
}

const Shader & Shader::defaultShader()
{
	return *s_default;
}
const Shader & Shader::positionalShader()
{
	return *s_positional;
}

const Shader & Shader::phongShader()
{
	return *s_phong;
}

GLuint Shader::defaultShaderID()
{
	return s_default->m_programID;
}

GLuint Shader::positionalShaderID()
{
	return s_positional->m_programID;
}

GLuint Shader::phongShaderID()
{
	return s_phong->m_programID;
}

GLuint Shader::programID() const
{
	return m_programID;
}

Shader::~Shader() { }
