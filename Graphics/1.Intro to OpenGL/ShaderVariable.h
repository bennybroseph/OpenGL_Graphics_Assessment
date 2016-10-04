#ifndef _SHADER_VARIABLE_H_
#define _SHADER_VARIABLE_H_
#pragma once

#include <gl_core_4_4.h>

#include "MasterHeader.h"
#include <bitset>

#define toBit(value) static_cast<int>(value);

using std::bitset;

class ShaderVariable;

typedef unique_ptr<ShaderVariable> ShaderVariablePtrU;
typedef shared_ptr<ShaderVariable> ShaderVariablePtrS;
typedef weak_ptr<ShaderVariable> ShaderVariablePtrW;

class ShaderVariable
{
	enum class VariableType
	{
		Float,
		Vector2,
		Vector3,
		Vector4,
		Sampler2D,
	};

	enum class ParameterType
	{
		None = 0,

		Colour = 1 << 0,
		Range = 1 << 1,
		Min = 1 << 2,
		Max = 1 << 3,
	};

	class Parameter;

	typedef unique_ptr<Parameter> ParameterPtrU;
	typedef shared_ptr<Parameter> ParameterPtrS;
	typedef weak_ptr<Parameter> ParameterPtrW;

	struct Parameter
	{
		bitset<4> type;
		vectorPtrU<GLfloat> values = make_unique<vector<GLfloat>>();
	};

public:

	ShaderVariable() { }
	ShaderVariable(GLuint shader, void *data, const char name[], const char displayName[], VariableType type);

	void drawGui() const;
	void setUniform() const;

	void parseType(const string &text);

private:

	GLuint m_shader = 0;

	mutable shared_ptr<void> m_data = shared_ptr<void>();
	vectorPtrU<Parameter> m_parameters = make_unique<vector<Parameter>>();

	unique_ptr<GLchar> m_name = unique_ptr<GLchar>(new GLchar[255]);
	unique_ptr<GLchar> m_displayName = unique_ptr<GLchar>(new GLchar[255]);

	VariableType m_type;

};

#endif // _SHADER_VARIABLE_H_

