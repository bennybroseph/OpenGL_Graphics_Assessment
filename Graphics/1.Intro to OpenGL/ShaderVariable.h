#ifndef _SHADER_VARIABLE_H_
#define _SHADER_VARIABLE_H_
#pragma once

#include <gl_core_4_4.h>

#include "MasterHeader.h"

enum class VariableType
{
	Float,
	Vector2,
	Vector3,
	Vector4,
	Sampler2D,
};


class ShaderVariable;

typedef unique_ptr<ShaderVariable> ShaderVariablePtrU;
typedef shared_ptr<ShaderVariable> ShaderVariablePtrS;
typedef weak_ptr<ShaderVariable> ShaderVariablePtrW;

class ShaderVariable
{
public:

	ShaderVariable(void *data, const char name[], VariableType type);

	void drawGui() const;

	~ShaderVariable();

private:

	shared_ptr<void> m_data = shared_ptr<void>();
	unique_ptr<GLchar> m_name = unique_ptr<GLchar>(new GLchar[255]);
	VariableType m_type;

};

#endif // _SHADER_VARIABLE_H_

