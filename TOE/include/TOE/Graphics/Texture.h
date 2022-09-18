#pragma once

#include <GL/glew.h>
#include <string>

namespace TOE
{
	enum class TextureType
	{
		Diffuse,
		Specular,
		Normal
	};

	class Texture2D
	{
	public:
		Texture2D();
		~Texture2D();

		void CreateFromFile(const std::string& path);

		void Use(unsigned int slot = 0) const;
		unsigned int GetID() const;

		TextureType Type;

	private:
		unsigned int m_ID;
	};
}