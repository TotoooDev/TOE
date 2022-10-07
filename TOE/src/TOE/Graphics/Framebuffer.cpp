#include "pch.h"
#include <TOE/Graphics/Framebuffer.h>
#include <TOE/Core/Core.h>
#include <GL/glew.h>

namespace TOE
{
	Framebuffer::Framebuffer(const FramebufferSpecification& spec)
		: m_Spec(spec)
	{
		Recreate();
	}

	void Framebuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_ID);
	}

	void Framebuffer::Unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void Framebuffer::Recreate()
	{
		if (m_ID != -1)
			glDeleteFramebuffers(1, &m_ID);

		glGenFramebuffers(1, &m_ID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_ID);

		m_ColorAttachments.clear();

		std::vector<GLenum> attachments;
		for (unsigned int i = 0; i < m_Spec.m_Textures.size(); i++)
		{
			GLenum format;
			GLenum type;
			bool isDepth = false;

			FramebufferTextureFormatToGL(m_Spec.m_Textures[i], &format, &type, &isDepth);

			unsigned int textureID;
			glGenTextures(1, &textureID);
			glBindTexture(GL_TEXTURE_2D, textureID);
			glTexImage2D(GL_TEXTURE_2D, 0, format, m_Spec.Width, m_Spec.Height, 0, GL_RGBA, type, nullptr);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, textureID, 0);
			attachments.push_back(GL_COLOR_ATTACHMENT0 + i);

			m_ColorAttachments.push_back(textureID);
		}

		if (m_Spec.m_HasDepthTexture)
		{
			unsigned int id;
			glGenTextures(1, &id);
			glBindTexture(GL_TEXTURE_2D, id);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_Spec.Width, m_Spec.Height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, id, 0);
			m_DepthAttachment = id;
			attachments.push_back(GL_COLOR_ATTACHMENT0 + attachments.size());

			m_ColorAttachments.push_back(id);
		}

		glDrawBuffers(attachments.size(), attachments.data());

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			spdlog::error("Framebuffer is not complete!");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void Framebuffer::Resize(unsigned int width, unsigned int height)
	{
		m_Spec.Width = width;
		m_Spec.Height = height;
		Recreate();
	}

	unsigned int Framebuffer::RetrieveTexture(unsigned int id)
	{
		return m_ColorAttachments[id];
	}

	void Framebuffer::BindTexture(unsigned int id, unsigned int slot)
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, m_ColorAttachments[id]);
	}

	void Framebuffer::BindAllTextures()
	{
		for (unsigned int i = 0; i < m_ColorAttachments.size(); i++)
		{
			BindTexture(i, i);
		}
	}

	void Framebuffer::ClearAttachmentTexture(unsigned int id, unsigned int value)
	{
		unsigned int format, type;
		bool isDepth;
		FramebufferTextureFormatToGL(m_Spec.m_Textures[id], &format, &type, &isDepth);
		glClearTexImage(m_ColorAttachments[id], 0, GL_RGBA, type, &value);
	}

	void Framebuffer::ClearAllAttachmentTextures(unsigned int value)
	{
		for (unsigned int i = 0; i < m_ColorAttachments.size(); i++)
		{
			ClearAttachmentTexture(i, value);
		}
	}

	void Framebuffer::FramebufferTextureFormatToGL(FramebufferTexture texture, unsigned int* format, unsigned int* type, bool* isDepth)
	{
		switch (texture)
		{
		default:								  *format = GL_RGBA8;            *type = GL_UNSIGNED_BYTE;                  break;
		case FramebufferTexture::RGBA8:           *format = GL_RGBA8;            *type = GL_UNSIGNED_BYTE;                  break;
		case FramebufferTexture::RGBA16:          *format = GL_RGBA16;           *type = GL_FLOAT;                          break;
		case FramebufferTexture::RGBA16F:         *format = GL_RGBA16F;          *type = GL_FLOAT;                          break;
		case FramebufferTexture::Depth24Stencil8: *format = GL_DEPTH24_STENCIL8; *type = GL_FLOAT;         *isDepth = true; break;
		}
	}
}