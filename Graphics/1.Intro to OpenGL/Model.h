#ifndef _MODEL_H_
#define _MODEL_H_
#pragma once

#include "Shader.h"
#include "Component.h"
#include "Texture.h"
#include "Mesh.h"

class Model;

typedef unique_ptr<Model> ModelPtrU;
typedef shared_ptr<Model> ModelPtrS;
typedef weak_ptr<Model> ModelPtrW;

class Model : public Component
{
public:

	Model();

	void draw() const override;
	void drawGizmos() const override;
	void drawGui() const override;

	void drawModel(const mat4 &matrix) const;

	const Shader * getShader() const;
	void setShader(const Shader *newShader) const;

	const vec4 & getMaterialColour() const;
	void setMaterialColour(const vec4 &newColour);

	int addTexture(const GLchar *path, FilteringType filteringType, const GLchar *name = "");
	int removeTextureAt(GLuint index);
	int removeTexture(GLuint handle);

	int setNormalTexture(const GLchar *path, FilteringType filteringType);
	int setDiffuseTexture(const GLchar *path, FilteringType filteringType);
	int setSpecularTexture(const GLchar *path, FilteringType filteringType);

	~Model();

	const Mesh *m_mesh = nullptr;

	mutable GLint m_drawType = GL_TRIANGLES;

	mutable GLboolean m_isEnabled = true;
	mutable GLboolean m_shouldDrawModel = true;
	mutable GLboolean m_shouldDrawWireFrame = false;

private:

	mutable const Shader *m_shader = Shader::standard();
	mutable vectorPtrU<ShaderVariablePtrU> m_shaderVariables = make_unique<vector<ShaderVariablePtrU>>();

	vec4PtrU m_materialColour = make_unique<vec4>(vec4(1.f, 1.f, 1.f, 1.f));

	vectorPtrU<TexturePtrU> m_textures = make_unique<vector<TexturePtrU>>();

	TexturePtrU m_normalTexture = make_unique<Texture>();
	TexturePtrU m_diffuseTexture = make_unique<Texture>();
	TexturePtrU m_specularTexture = make_unique<Texture>();

	GLuint m_perlin_texture;
};


#endif // _MODEL_H_
