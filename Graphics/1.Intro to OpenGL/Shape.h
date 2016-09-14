#ifndef _SHAPE_H_
#define _SHAPE_H_

#include "Shader.h"
#include "Transform.h"
#include "Model.h"

class Shape
{
public:

	virtual void draw();

	GLint& drawType();
	
	Shader& shader();
	
	vec4& materialColour();

	Transform& transform();

	bool& isEnabled();
	bool& shouldDrawModel();
	bool& shouldDrawWireFrame();

	virtual ~Shape();

protected:

	virtual void drawModel();

	GLint m_drawType = GL_TRIANGLES;

	Shader m_shader = Shader::defaultShader();

	vec4 m_materialColour = vec4(1.f, 1.f, 1.f, 1.f);

	Transform m_transform;

	Model *m_model = nullptr;

	bool m_isEnabled = true;
	bool m_shouldDrawModel = true;
	bool m_shouldDrawWireFrame = false;
};

#endif // _SHAPE_H_