#pragma once

#include <glm/glm.hpp>

namespace TOE
{
	enum LightType
	{
		DirectionnalLight,
		PointLight,
		SpotLight
	};

	struct Light
	{
		LightType Type;

		glm::vec3 Ambient = glm::vec3(0.2f);
		glm::vec3 Diffuse = glm::vec3(0.5f);
		glm::vec3 Specular = glm::vec3(1.0f);

		float Constant = 1.0f;
		float Linear = 0.14f;
		float Quadratic = 0.07f;

		float CutOff = 12.5f;
		float OuterCutOff = 17.5f;
	};
}