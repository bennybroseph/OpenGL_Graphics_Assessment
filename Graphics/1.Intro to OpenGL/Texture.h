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
	Texture(const GLchar *path, FilteringType filteringType);

	GLuint getHandle() const;
	int setTexture(const GLchar *path);

	FilteringType getFiltering() const;
	int setFiltering(FilteringType filteringType);

	~Texture();

private:

	GLuint m_handle = 0;

	FilteringType m_filtering = FilteringType::Nearest;

};

#endif // _TEXTURE_H_

