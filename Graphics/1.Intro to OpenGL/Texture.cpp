#include "Texture.h"

#include <stb_image.h>

Texture::Texture(const GLchar* path, FilteringType filteringType)
{
	setTexture(path);
	setFiltering(filteringType);
}

GLuint Texture::getHandle() const
{
	return m_handle;
}
int Texture::setTexture(const GLchar* path)
{
	if (m_handle != 0)
		glDeleteTextures(1, &m_handle);

	auto imageWidth = 0;
	auto imageHeight = 0;
	auto imageFormat = 0;

	auto data = stbi_load(path, &imageWidth, &imageHeight, &imageFormat, STBI_default);

	if (data == nullptr)
	{
		printf("stbi error loading %s: %s", path, stbi_failure_reason());
		return -1;
	}

	GLuint format;
	switch (imageFormat)
	{
	case 3:
		format = GL_RGB;
		break;
	case 4:
		format = GL_RGBA;
		break;

	default:
		return -1;
	}

	glGenTextures(1, &m_handle);
	glBindTexture(GL_TEXTURE_2D, m_handle);
	{
		glTexImage2D(GL_TEXTURE_2D, 0, format, imageWidth, imageHeight, 0, format, GL_UNSIGNED_BYTE, data);
	}
	glBindTexture(GL_TEXTURE_2D, 0);	// Unbind

	stbi_image_free(data);

	return 0;
}

FilteringType Texture::getFiltering() const
{
	return m_filtering;
}
int Texture::setFiltering(FilteringType filteringType)
{
	GLuint filtering;
	m_filtering = filteringType;
	switch (m_filtering)
	{
	case FilteringType::Nearest:
		filtering = GL_NEAREST;
		break;

	case FilteringType::Linear:
		filtering = GL_LINEAR;
		break;

	default:
		return -1;
	}

	glBindTexture(GL_TEXTURE_2D, m_handle);
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filtering);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filtering);
	}
	glBindTexture(GL_TEXTURE_2D, 0);	// Unbind

	return 0;
}

Texture::~Texture()
{
	glDeleteTextures(1, &m_handle);
}
