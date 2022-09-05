#pragma once

namespace TOE
{
	struct FramebufferData
	{
		unsigned int Width, Height;
	};

	class Framebuffer
	{
	public:
		~Framebuffer();

		void Create(const FramebufferData& data);
		// Apparently this causes a spike in the memory during a few seconds
		// No idea why this happens
		void Recreate();
		void Resize(unsigned int width, unsigned int height);

		void Use();
		void Unbind();

		const FramebufferData& GetFramebufferData();
		unsigned int GetID();
		unsigned int GetColorAttachmentID();

	private:
		FramebufferData m_Data;

		unsigned int m_ID = 0;
		unsigned int m_ColorAttachmentID = 0;
	};
}