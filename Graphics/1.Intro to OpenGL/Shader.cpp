#include "Shader.h"

#include <fstream>
#include <string>

using std::fstream;
using std::string;

const string Shader::EDITABLE_IDENTIFIER = "// Inspector";

ShaderPtrU Shader::s_standard = unique_ptr<Shader>();
ShaderPtrU Shader::s_basic = unique_ptr<Shader>();
ShaderPtrU Shader::s_texture = unique_ptr<Shader>();
ShaderPtrU Shader::s_positional = unique_ptr<Shader>();
ShaderPtrU Shader::s_phong = unique_ptr<Shader>();
ShaderPtrU Shader::s_perlin = unique_ptr<Shader>();

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

	s_basic.reset(new Shader());
	s_basic->setName("Basic");

	returnValue = s_basic->addShader("Basic.vert", ShaderType::Vertex);
	if (returnValue != 0)
		return returnValue;
	returnValue = s_basic->addShader("Basic.frag", ShaderType::Fragment);
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

	s_perlin.reset(new Shader());
	s_perlin->setName("Perlin");

	returnValue = s_perlin->addShader("Perlin.vert", ShaderType::Vertex);
	if (returnValue != 0)
		return returnValue;
	returnValue = s_perlin->addShader("Texture.frag", ShaderType::Fragment);
	if (returnValue != 0)
		return returnValue;

	return returnValue;
}

int Shader::quit()
{
	s_standard.reset();
	s_basic.reset();
	s_texture.reset();
	s_positional.reset();
	s_phong.reset();
	s_perlin.reset();

	return 0;
}

Shader::Shader()
{
	if (s_shaders.get() == nullptr)
		s_shaders.reset(new vector<Shader *>);

	m_programID = glCreateProgram();

	s_shaders->push_back(this);	// Add itself to the list of objects
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

	m_shaderPaths->push_back(make_unique<string>(path));

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
const Shader * Shader::basic()
{
	return s_basic.get();
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

const Shader * Shader::perlin()
{
	return s_perlin.get();
}

const vector<Shader *> * Shader::getShaders()
{
	return s_shaders.get();
}

vectorPtrU<ShaderVariablePtrU> Shader::getVariables() const
{
	auto tempVector = make_unique<vector<ShaderVariablePtrU>>();

	for (auto &path : *m_shaderPaths)
	{
		fstream shaderFile;
		shaderFile.open(*path, std::ios::in);

		string parsedText = "";
		string line;
		while (getline(shaderFile, line))
			parsedText += line;

		shaderFile.close();

		int editablePosition = parsedText.find(EDITABLE_IDENTIFIER);
		while (editablePosition != string::npos)
		{
			if (editablePosition != string::npos)
			{
				auto subString = parsedText.substr(editablePosition + EDITABLE_IDENTIFIER.length());
				auto parameters = subString.substr(0, subString.find("uniform "));

				auto t = ShaderVariable();
				t.parseType(parsedText.substr(editablePosition, parsedText.find(";")));

				subString = subString.substr(
					subString.find("uniform ") != string::npos ? string("uniform ").length() : 0);

				subString = subString.substr(0, subString.find(";"));

				auto variableType = subString.substr(subString.find(" "));

				variableType = subString.substr(0, subString.find(" "));

				int equals = subString.find("=");
				auto variableName = subString.substr(subString.find(" ") + 1);
				variableName = variableName.substr(
					0,
					variableName.find("=") != string::npos ?
					variableName.find("=") : variableName.find(";"));

				while (variableName.find(" ") != string::npos)
					variableName.replace(variableName.find(" "), 1, "");

				auto displayName = variableName;
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

				if (variableType == "float")
				{
					auto newFloat = new GLfloat(0.f);
					if (equals != string::npos)
						*newFloat = atof(subString.substr(equals + 1, subString.find(";")).c_str());

					/*tempVector->push_back(
						make_unique<ShaderVariable>(
							m_programID,
							newFloat,
							variableName.c_str(),
							displayName.c_str(),
							VariableType::Float));*/
				}

				if (variableType == "vec2")
				{
					auto newVector = new vec2(0.f);
					if (equals != string::npos)
					{
						int currentFind = subString.find("(") + 1;

						GLfloat values[2];
						for (auto i = 0; i < 2; ++i)
						{
							if (subString.find(",") != string::npos)
								values[i] = atof(subString.substr(currentFind, subString.find(",")).c_str());
							else
								values[i] = atof(subString.substr(currentFind, subString.find(")")).c_str());
							currentFind = subString.find(",", currentFind) + 1;
						}
						*newVector = vec2(values[0], values[1]);
					}

					/*tempVector->push_back(
						make_unique<ShaderVariable>(
							m_programID,
							newVector,
							variableName.c_str(),
							displayName.c_str(),
							VariableType::Vector2));*/
				}

				if (variableType == "vec3")
				{
					auto newVector = new vec3(0.f);
					if (equals != string::npos)
					{
						int currentFind = subString.find("(") + 1;

						GLfloat values[3];
						for (auto i = 0; i < 3; ++i)
						{
							if (subString.find(",") != string::npos)
								values[i] = atof(subString.substr(currentFind, subString.find(",")).c_str());
							else
								values[i] = atof(subString.substr(currentFind, subString.find(")")).c_str());
							currentFind = subString.find(",", currentFind) + 1;
						}
						*newVector = vec3(values[0], values[1], values[2]);
					}

					/*tempVector->push_back(
						make_unique<ShaderVariable>(
							m_programID,
							newVector,
							variableName.c_str(),
							displayName.c_str(),
							VariableType::Vector3));*/
				}

				if (variableType == "vec4")
				{
					auto newVector = new vec4(0.f);
					if (equals != string::npos)
					{
						int currentFind = subString.find("(") + 1;

						GLfloat values[4];
						for (auto i = 0; i < 4; ++i)
						{
							if (subString.find(",") != string::npos)
								values[i] = atof(subString.substr(currentFind, subString.find(",")).c_str());
							else
								values[i] = atof(subString.substr(currentFind, subString.find(")")).c_str());
							currentFind = subString.find(",", currentFind) + 1;
						}
						*newVector = vec4(values[0], values[1], values[2], values[3]);
					}

					/*tempVector->push_back(
						make_unique<ShaderVariable>(
							m_programID,
							newVector,
							variableName.c_str(),
							displayName.c_str(),
							VariableType::Vector4));*/
				}
			}

			parsedText = parsedText.substr(parsedText.find("uniform ", editablePosition));
			editablePosition = parsedText.find(EDITABLE_IDENTIFIER);
		}
	}

	return tempVector;
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
