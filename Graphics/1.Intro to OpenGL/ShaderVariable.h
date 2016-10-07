#ifndef _SHADER_VARIABLE_H_
#define _SHADER_VARIABLE_H_
#pragma once

#include <gl_core_4_4.h>

#include "MasterHeader.h"

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
		Colour,
		Range,
		Min,
		Max,
		Step,
	};

	struct Parameter;

	typedef unique_ptr<Parameter> ParameterPtrU;
	typedef shared_ptr<Parameter> ParameterPtrS;
	typedef weak_ptr<Parameter> ParameterPtrW;

	struct Parameter
	{
		ParameterType type;
		vectorPtrU<GLfloat> values = make_unique<vector<GLfloat>>();
	};

public:

	ShaderVariable(GLuint shader, const string &text);

	void drawGui() const;
	void setUniform() const;

	void parseType(const string &text);
	void parseParameters(const string &text);
	void parseName(const string &text);

	template<typename T>
	/// <summary>
	/// This is very dangerous as there is no way to type check.
	/// Sets the value of the void pointer to whatever value is passed in.
	/// A copy of the value is made in order to complete this action
	/// </summary>
	/// <param name="newValue">The new value to be used for this variable</param>
	void setVariable(T newValue)
	{
		*m_data = newValue;
	}

private:

	GLuint m_shader = 0;

	mutable shared_ptr<void> m_data = shared_ptr<void>();
	vectorPtrU<ParameterPtrU> m_parameters = make_unique<vector<ParameterPtrU>>();

	unique_ptr<GLchar> m_name = unique_ptr<GLchar>(new GLchar[255]);
	unique_ptr<GLchar> m_displayName = unique_ptr<GLchar>(new GLchar[255]);

	VariableType m_type;

};

#endif // _SHADER_VARIABLE_H_

