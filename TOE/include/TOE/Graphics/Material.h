#pragma once

#include <TOE/Core/Ref.h>
#include <TOE/Graphics/Texture.h>

#include <glm/glm.hpp>

namespace TOE
{
	class Material
	{
	public:
		Ref<Texture2D> Diffuse = CreateRef<Texture2D>();
		Ref<Texture2D> Specular = CreateRef<Texture2D>();
		Ref<Texture2D> Normal = CreateRef<Texture2D>();
		float Shininess = 32.0f;

		std::string Name = "";
	};
}