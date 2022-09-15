#pragma once

#include <TOE/Core/Ref.h>
#include <TOE/Graphics/Texture.h>

#include <glm/glm.hpp>

namespace TOE
{
	class Material
	{
	public:
		glm::vec3 DiffuseColor = glm::vec3(1.0f, 0.0f, 1.0f);

		Ref<Texture2D> Diffuse;
		Ref<Texture2D> Specular;
		float Shininess = 32.0f;
	};
}