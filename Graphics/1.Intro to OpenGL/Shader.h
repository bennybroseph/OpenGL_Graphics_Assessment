#ifndef _SHADERUTILITY_H_
#define _SHADERUTILITY_H_

#include <gl_core_4_4.h>

#include <string>

using std::string;

enum class ShaderType
{
	Vertex,
	Fragment,
};

class Shader
{
public:

	Shader();
	static int init();

	int createHandle();

	int addShader(string path, ShaderType type);

	static const Shader &defaultShader();
	static const GLuint &defaultShaderID();

	const GLuint& programID() const;

	~Shader();	

private:

	static Shader s_defaultShader;
	static GLuint &s_defaultShaderID;

	GLuint m_programID;

};

#endif // _SHADERUTILITY_H_
