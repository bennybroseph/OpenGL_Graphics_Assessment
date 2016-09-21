#ifndef _SHADERUTILITY_H_
#define _SHADERUTILITY_H_

#include <gl_core_4_4.h>

#include <string>

using std::string;

enum class ShaderType
{
	Vertex,
	Fragment,
	Geometry,
};

class Shader
{
public:

	Shader();
	static int init();

	int addShader(string path, ShaderType type) const;

	static const Shader& defaultShader();
	static const Shader& positionalShader();
	static const Shader& phongShader();

	static const GLuint& defaultShaderID();
	static const GLuint& positionalShaderID();
	static const GLuint& phongShaderID();

	const GLuint& programID() const;

	~Shader();

private:

	static Shader s_default;
	static Shader s_positional;
	static Shader s_phong;

	GLuint m_programID;

};

#endif // _SHADERUTILITY_H_
