#include "ShaderVariable.h"

#include <imgui_impl_glfw_gl3.h>

ShaderVariable::ShaderVariable(void *data, const char *name, VariableType type)
{
	m_data.reset(data);
	_memccpy(m_name.get(), name, 0, 255);
	m_type = type;
}


void ShaderVariable::drawGui() const
{
	switch (m_type)
	{
	case VariableType::Float:
	{
		auto tempFloat = static_cast<GLfloat *>(m_data.get());
		ImGui::DragFloat(m_name.get(), tempFloat);
	}
	break;

	default:
		break;
	}
}

ShaderVariable::~ShaderVariable()
{
}
