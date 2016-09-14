#include "Shader.h"

#include <fstream>

using std::fstream;

Shader Shader::s_defaultShader = Shader();
Shader Shader::s_positionShader = Shader();

Shader::Shader() { }

int Shader::init()
{
	s_defaultShader.m_programID = glCreateProgram();

	auto returnValue = s_defaultShader.addShader("DefaultVertexShader.glsl", ShaderType::Vertex);
	if (returnValue != 0)
		return returnValue;

	returnValue = s_defaultShader.addShader("DefaultFragmentShader.glsl", ShaderType::Fragment);
	if (returnValue != 0)
		return returnValue;

	s_positionShader.m_programID = glCreateProgram();

	returnValue = s_positionShader.addShader("PositionVertexShader.glsl", ShaderType::Vertex);
	if (returnValue != 0)
		return returnValue;

	returnValue = s_positionShader.addShader("DefaultFragmentShader.glsl", ShaderType::Fragment);
	if (returnValue != 0)
		return returnValue;

	return returnValue;
}

int Shader::addShader(string path, ShaderType type)
{
	auto parsedType = type == ShaderType::Vertex ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER;

	fstream shaderFile;
	shaderFile.open(path, std::ios::in);

	string parsedText = "";
	string line;
	while (getline(shaderFile, line))
	{
		if (parsedText == "")
			line += "\n";
		parsedText += line + " ";
	}
	shaderFile.close();

	auto vertexShaderSource = parsedText.c_str();

	auto shader = glCreateShader(parsedType);
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
			"Error: Failed to compile %s shader at path %s!\n",
			type == ShaderType::Vertex ? "vertex" : "fragment",
			path.c_str());
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
		printf("Error: Failed to link shader program!\n");
		printf("%s\n", infoLog);
		delete[] infoLog;

		return -1;
	}

	glDeleteShader(shader);

	return 0;
}

const Shader & Shader::defaultShader()
{
	return s_defaultShader;
}
const Shader & Shader::positionShader()
{
	return s_positionShader;
}

const GLuint & Shader::defaultShaderID()
{
	return s_defaultShader.m_programID;
}

const GLuint & Shader::positionShaderID()
{
	return s_positionShader.m_programID;
}

const GLuint & Shader::programID() const
{
	return m_programID;
}

Shader::~Shader() { }
