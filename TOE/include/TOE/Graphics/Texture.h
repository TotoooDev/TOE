#pragma once

#include <GL/glew.h>
#include <string>

namespace TOE
{
	class Texture2D
	{
	public:
		Texture2D();
		~Texture2D();

		void CreateFromFile(const std::string& path);

		std::string GetPath();

		void Use(unsigned int slot = 0) const;
		unsigned int GetID() const;

	private:
		unsigned int m_ID;
		std::string m_Path;
	};
}