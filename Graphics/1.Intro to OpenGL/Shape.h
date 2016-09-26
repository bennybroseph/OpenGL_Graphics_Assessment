#ifndef _SHAPE_H_
#define _SHAPE_H_
#pragma once

#include "Shader.h"
#include "Transform.h"
#include "Mesh.h"

class Shape;

typedef unique_ptr<Shape> ShapePtrU;
typedef shared_ptr<Shape> ShapePtrS;
typedef weak_ptr<Shape> ShapePtrW;

class Shape
{
public:

	virtual void draw();

	const Shader * getShader() const;
	void setShader(const Shader *newShader);

	const vec4 & getMaterialColour() const;
	void setMaterialColour(const vec4 &newColour);

	Transform * transform();
	const Transform * transform() const;

	GLint * drawType() const;

	GLboolean * isEnabled() const;
	GLboolean * shouldDrawModel() const;
	GLboolean * shouldDrawWireFrame() const;

	virtual ~Shape();

protected:

	virtual void drawModel();

	const Shader *m_shader = &Shader::standard();

	const vec4PtrU m_materialColour = make_unique<vec4>(vec4(1.f, 1.f, 1.f, 1.f));

	const TransformPtrU m_transform = make_unique<Transform>();

	const Mesh *m_mesh = nullptr;

	mutable GLint m_drawType = GL_TRIANGLES;

	mutable GLboolean m_isEnabled = true;
	mutable GLboolean m_shouldDrawModel = true;
	mutable GLboolean m_shouldDrawWireFrame = false;
};

#endif // _SHAPE_H_