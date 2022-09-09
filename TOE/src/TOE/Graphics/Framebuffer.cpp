#include <TOE/Graphics/Framebuffer.h>
#include <TOE/Core/Core.h>
#include <GL/glew.h>

namespace TOE
{
	Framebuffer::~Framebuffer()
	{
		glDeleteFramebuffers(1, &m_ID);
		glDeleteTextures(1, &m_ColorAttachmentID);
		glDeleteRenderbuffers(1, &m_RenderBufferID);
	}

	void Framebuffer::Create(const FramebufferData& data)
	{
		m_Data = data;
		Recreate();
	}

	void Framebuffer::Recreate()
	{
		// If the framebuffer already exists we destroy it
		if (m_ID != 0)
		{
			glDeleteFramebuffers(1, &m_ID);
			m_ID = 0;
		}
		if (m_RenderBufferID != 0)
		{
			glDeleteRenderbuffers(1, &m_RenderBufferID);
			m_RenderBufferID = 0;
		}
		// Same for the texture
		if (m_ColorAttachmentID != 0)
		{
			glDeleteTextures(1, &m_ColorAttachmentID);
			m_ColorAttachmentID = 0;
		}

		// Create the framebuffer
		glCreateFramebuffers(1, &m_ID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_ID);

		// Create the texture attachment
		glCreateTextures(GL_TEXTURE_2D, 1, &m_ColorAttachmentID);
		glBindTexture(GL_TEXTURE_2D, m_ColorAttachmentID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Data.Width, m_Data.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glGenRenderbuffers(1, &m_RenderBufferID);
		glBindRenderbuffer(GL_RENDERBUFFER, m_RenderBufferID);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_Data.Width, m_Data.Height);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RenderBufferID);

		// Bind the texture to the framebuffer
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttachmentID, 0);
		
		TOE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is not complete!");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void Framebuffer::Resize(unsigned int width, unsigned int height)
	{
		m_Data.Width = width;
		m_Data.Height = height;
		Recreate();
	}

	void Framebuffer::Use()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_ID);
		glViewport(0, 0, m_Data.Width, m_Data.Height);
	}

	void Framebuffer::Unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	const FramebufferData& Framebuffer::GetFramebufferData()
	{
		return m_Data;
	}

	unsigned int Framebuffer::GetID()
	{
		return m_ID;
	}

	unsigned int Framebuffer::GetColorAttachmentID()
	{
		return m_ColorAttachmentID;
	}
}