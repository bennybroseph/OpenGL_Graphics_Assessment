#include "Shader.h"

#include <fstream>
#include <string>

using std::fstream;
using std::string;

ShaderPtrU Shader::s_standard = unique_ptr<Shader>();
ShaderPtrU Shader::s_texture = unique_ptr<Shader>();
ShaderPtrU Shader::s_positional = unique_ptr<Shader>();
ShaderPtrU Shader::s_phong = unique_ptr<Shader>();

vectorPtrU<Shader *> Shader::s_shaders = unique_ptr<vector<Shader *>>();

int Shader::init()
{
	s_standard.reset(new Shader());
	s_standard->setName("Standard");

	auto returnValue = s_standard->addShader("Default.vert", ShaderType::Vertex);
	if (returnValue != 0)
		return returnValue;
	returnValue = s_standard->addShader("Default.frag", ShaderType::Fragment);
	if (returnValue != 0)
		return returnValue;

	s_texture.reset(new Shader());
	s_texture->setName("Texture");

	returnValue = s_texture->addShader("Texture.vert", ShaderType::Vertex);
	if (returnValue != 0)
		return returnValue;
	returnValue = s_texture->addShader("Texture.frag", ShaderType::Fragment);
	if (returnValue != 0)
		return returnValue;

	s_positional.reset(new Shader());
	s_positional->setName("Positional");

	returnValue = s_positional->addShader("Positional.vert", ShaderType::Vertex);
	if (returnValue != 0)
		return returnValue;
	returnValue = s_positional->addShader("Default.frag", ShaderType::Fragment);
	if (returnValue != 0)
		return returnValue;

	s_phong.reset(new Shader());
	s_phong->setName("Phong");

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
	s_standard.reset();
	s_texture.reset();
	s_positional.reset();
	s_phong.reset();

	return 0;
}

Shader::Shader()
{
	if (s_shaders.get() == nullptr)
		s_shaders.reset(new vector<Shader *>);

	s_shaders->push_back(this);	// Add itself to the list of objects

	m_programID = glCreateProgram();
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

const GLchar * Shader::getName() const
{
	return m_name.get();
}
void Shader::setName(const GLchar *newName)
{
	_memccpy(m_name.get(), newName, 0, 255);
}

const Shader * Shader::standard()
{
	return s_standard.get();
}
const Shader * Shader::texture()
{
	return s_texture.get();
}
const Shader * Shader::positional()
{
	return s_positional.get();
}
const Shader * Shader::phong()
{
	return s_phong.get();
}

const vector<Shader *> * Shader::getShaders()
{
	return s_shaders.get();
}

GLuint Shader::programID() const
{
	return m_programID;
}

Shader::~Shader()
{
	glDeleteProgram(m_programID);

	s_shaders->erase(find(s_shaders->begin(), s_shaders->end(), this));	// Remove itself from the list of objects

	if (s_shaders->size() == 0)
		s_shaders.reset();
}
