#ifndef _MODEL_H_
#define _MODEL_H_
#pragma once

#include "Shader.h"
#include "Transform.h"
#include "Mesh.h"

class Model;

typedef unique_ptr<Model> ModelPtrU;
typedef shared_ptr<Model> ModelPtrS;
typedef weak_ptr<Model> ModelPtrW;

class Model
{
public:

	void draw() const;

	const Shader * getShader() const;
	void setShader(const Shader *newShader);

	const vec4 & getMaterialColour() const;
	void setMaterialColour(const vec4 &newColour);

	Transform * transform();
	const Transform * transform() const;

	~Model();

	Mesh m_mesh;

	mutable GLint m_drawType = GL_TRIANGLES;

	mutable GLboolean m_isEnabled = true;
	mutable GLboolean m_shouldDrawModel = true;
	mutable GLboolean m_shouldDrawWireFrame = false;

private:

	void drawModel() const;

	mutable const Shader *m_shader = &Shader::standard();

	vec4PtrU m_materialColour = make_unique<vec4>(vec4(1.f, 1.f, 1.f, 1.f));

	TransformPtrU m_transform = make_unique<Transform>();

};


#endif // _MODEL_H_
