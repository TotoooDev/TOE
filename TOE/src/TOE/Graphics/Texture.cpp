#include "pch.h"
#include <TOE/Graphics/Texture.h>
#include <stb_image/stb_image.h>

namespace TOE
{
	Texture2D::Texture2D()
	{
		TOE_PROFILE_FUNCTION();

		stbi_set_flip_vertically_on_load(true);
		glGenTextures(1, &m_ID);
	}

	Texture2D::~Texture2D()
	{
		glDeleteTextures(1, &m_ID);
	}

	void Texture2D::CreateFromFile(const std::string& path)
	{
		TOE_PROFILE_FUNCTION();

		glBindTexture(GL_TEXTURE_2D, m_ID);

		// Set the texture wrapping parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// Set texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		int width, height, numChannels;
		unsigned char* data = stbi_load(path.c_str(), &width, &height, &numChannels, 0);
		if (data == nullptr)
		{
			spdlog::error("Failed to load image {}!", path);
			return;
		}
		m_Path = path;

		GLenum glFormat = GL_RGB;
		if (numChannels == 3)
		{
			glFormat = GL_RGB;
		}
		if (numChannels == 4)
		{
			glFormat = GL_RGBA;
		}
		glTexImage2D(GL_TEXTURE_2D, 0, glFormat, width, height, 0, glFormat, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		stbi_image_free(data);
	}

	std::string Texture2D::GetPath()
	{
		return m_Path;
	}

	void Texture2D::Use(unsigned int slot) const
	{
		TOE_PROFILE_FUNCTION();

		// Activate the texture unit first before binding texture
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, m_ID);
	}

	unsigned int Texture2D::GetID() const
	{
		return m_ID;
	}
}