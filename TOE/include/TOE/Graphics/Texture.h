#pragma once

#include <GL/glew.h>
#include <string>

namespace TOE
{
	class Texture2D
	{
	public:
		Texture2D();

		void CreateFromFile(const std::string& path);

		void Use(unsigned int slot = 0);
		unsigned int GetID();

	private:
		unsigned int m_ID;
	};
}