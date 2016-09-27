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

	static const Shader & standard();
	static const Shader & texture();
	static const Shader & positional();
	static const Shader & phong();

	static GLuint standardID();
	static GLuint textureID();
	static GLuint positionalID();
	static GLuint phongID();

	GLuint programID() const;

	~Shader();

private:

	static ShaderPtrU s_standard;
	static ShaderPtrU s_texture;
	static ShaderPtrU s_positional;
	static ShaderPtrU s_phong;

	GLuint m_programID = 0;
};

#endif // _SHADERUTILITY_H_
