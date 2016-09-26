#ifndef _SHADERUTILITY_H_
#define _SHADERUTILITY_H_
#pragma once

#include <gl_core_4_4.h>

#include <MasterHeader.h>

enum class ShaderType
{
	Vertex,
	Fragment,
	Geometry,
};

class Shader;

typedef unique_ptr<Shader> ShaderPtrU;
typedef shared_ptr<Shader> ShaderPtrS;
typedef weak_ptr<Shader> ShaderPtrW;

class Shader
{
public:

	static int init();
	static int quit();

	int addShader(const char *path, ShaderType type) const;

	static const Shader & defaultShader();
	static const Shader & positionalShader();
	static const Shader & phongShader();

	static GLuint defaultShaderID();
	static GLuint positionalShaderID();
	static GLuint phongShaderID();

	GLuint programID() const;

	~Shader();

private:

	static ShaderPtrU s_default;
	static ShaderPtrU s_positional;
	static ShaderPtrU s_phong;

	GLuint m_programID = 0;
};

#endif // _SHADERUTILITY_H_
