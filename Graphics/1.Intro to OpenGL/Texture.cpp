#include "Texture.h"

#include <stb_image.h>
#include <windows.h>
#include <Commdlg.h>
#include <imgui.h>

Texture::Texture(const GLchar* path, FilteringType filteringType, const GLchar * name)
{
	setTexture(path);
	setFiltering(filteringType);
	setName(name);
}

void Texture::drawGui()
{
	if (!m_handle)
		return;

	ImGui::PushID(this);
	{
		auto collapsed = ImGui::CollapsingHeader(m_name.get(), nullptr, false);
		if (collapsed)
		{
			ImGui::Indent();
			{
				auto imageWidth = ImGui::GetWindowWidth() / 5.f;
				auto aspectRatio = m_imageHeight / m_imageWidth;
				ImGui::Image(
					reinterpret_cast<ImTextureID>(m_handle),
					ImVec2(
						imageWidth,
						imageWidth * aspectRatio));

				ImGui::SameLine();
				GLchar tempPath[255];
				_memccpy(tempPath, m_path.get(), 0, 255);
				ImGui::PushItemWidth(-40);
				ImGui::InputText("", tempPath, 255, ImGuiInputTextFlags_ReadOnly);
				ImGui::PopItemWidth();

				ImGui::SameLine();
				if (ImGui::Button("..."))
				{
					char szFile[255] = "";
					OPENFILENAMEA ofn;

					ZeroMemory(&ofn, sizeof(ofn));
					ofn.lStructSize = sizeof(ofn);
					ofn.hwndOwner = nullptr;
					ofn.lpstrFile = szFile;
					ofn.lpstrFile[0] = '\0';
					ofn.nMaxFile = sizeof(szFile);
					ofn.lpstrFilter = TEXT("All Formats\0\0");
					ofn.nFilterIndex = 1;
					ofn.lpstrFileTitle = nullptr;
					ofn.nMaxFileTitle = 0;
					ofn.lpstrInitialDir = "data\\textures";
					ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

					if (GetOpenFileNameA(&ofn))
					{
						setTexture(szFile);
						setFiltering(m_filtering);
					}
				}
			}
			ImGui::Unindent();
		}
	}
	ImGui::PopID();
}

GLuint Texture::getHandle() const
{
	return m_handle;
}
int Texture::setTexture(const GLchar* path)
{
	if (m_handle != 0)
		glDeleteTextures(1, &m_handle);

	auto data = stbi_load(path, &m_imageWidth, &m_imageHeight, &m_imageFormat, STBI_default);

	if (data == nullptr)
	{
		printf("stbi error loading %s: %s", path, stbi_failure_reason());
		return -1;
	}

	GLuint format;
	switch (m_imageFormat)
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
		glTexImage2D(GL_TEXTURE_2D, 0, format, m_imageWidth, m_imageHeight, 0, format, GL_UNSIGNED_BYTE, data);
	}
	glBindTexture(GL_TEXTURE_2D, 0);	// Unbind

	_memccpy(m_path.get(), path, 0, 255);
	stbi_image_free(data);

	return 0;
}

const GLchar * Texture::getName() const
{
	return m_name.get();
}

void Texture::setName(const GLchar *newName) const
{
	_memccpy(m_name.get(), newName, 0, 255);
}

FilteringType Texture::getFiltering() const
{
	return m_filtering;
}
int Texture::setFiltering(FilteringType filteringType) const
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
