#ifndef _TEXTURE_H_
#define _TEXTURE_H_
#pragma once

#include <gl_core_4_4.h>

#include "MasterHeader.h"

enum class FilteringType
{
	Nearest,
	Linear
};

class Texture;

typedef unique_ptr<Texture> TexturePtrU;
typedef shared_ptr<Texture> TexturePtrS;
typedef weak_ptr<Texture> TexturePtrW;

class Texture
{

	friend class Mesh;

public:

	Texture() = default;
	Texture(const GLchar *path, FilteringType filteringType, const GLchar * name);

	void drawGui();

	GLuint getHandle() const;
	int setTexture(const GLchar *path);

	const GLchar * getName() const;
	void setName(const GLchar *newName) const;

	FilteringType getFiltering() const;
	int setFiltering(FilteringType filteringType) const;

	~Texture();

private:

	unique_ptr<GLchar[]> m_name = make_unique<GLchar[]>(255);
	GLuint m_handle = 0;

	unique_ptr<GLchar[]> m_path = make_unique<GLchar[]>(255);
	mutable FilteringType m_filtering = FilteringType::Nearest;

	GLint m_imageWidth = 0;
	GLint m_imageHeight = 0;
	GLint m_imageFormat = 0;

};

#endif // _TEXTURE_H_

