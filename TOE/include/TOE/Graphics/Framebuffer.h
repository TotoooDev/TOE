#pragma once

#include <vector>

namespace TOE
{
	// Like we say in France j'ai volé ce code sans aucune vergogne
	// https://github.com/TheCherno/Hazel/blob/master/Hazel/src/Hazel/Renderer/Framebuffer.h

	enum class FramebufferTextureFormat
	{
		None,
		// Color
		RGBA8,
		RedInteger,
		// Depth / Stencil
		Depth24Stencil8,
		// Defaults
		Default = Depth24Stencil8
	};

	struct FramebufferTextureSpecification
	{
		FramebufferTextureFormat TextureFormat = FramebufferTextureFormat::None;

		FramebufferTextureSpecification() = default;
		FramebufferTextureSpecification(FramebufferTextureFormat format)
			: TextureFormat(format) {}
	};

	struct FramebufferAttachementSpecification
	{
		std::vector<FramebufferTextureSpecification> Attachments;

		FramebufferAttachementSpecification() = default;
		FramebufferAttachementSpecification(std::initializer_list<FramebufferTextureSpecification> attachments)
			: Attachments(attachments) {}
	};

	struct FramebufferSpecification
	{
		unsigned int Width = 0, Height = 0;
		FramebufferAttachementSpecification Attachments;
		unsigned int Samples = 1;
	};

	class Framebuffer
	{
	public:
		// Framebuffer() = default;
		Framebuffer(const FramebufferSpecification& specs);
		~Framebuffer();

		void Recreate();
		void Resize(unsigned int width, unsigned int height);

		void Use();
		void Unbind();

		int ReadPixel(unsigned int attachmentIndex, unsigned int x, unsigned int y);
		void ClearAttachment(unsigned int attachmentIndex, int value);

		unsigned int GetColorAttachmentID(unsigned int index = 0) const;

		const FramebufferSpecification& GetSpecification() { return m_Specification; }
		unsigned int GetID() { return m_ID; }

	private:
		FramebufferSpecification m_Specification;
		unsigned int m_ID = 0;

		std::vector<FramebufferTextureSpecification> m_ColorAttachmentSpecifications;
		FramebufferTextureSpecification m_DepthAttachmentSpecification = FramebufferTextureFormat::None;

		std::vector<unsigned int> m_ColorAttachments;
		unsigned int m_DepthAttachment = 0;
	};
}