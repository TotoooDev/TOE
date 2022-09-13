#pragma once

#include <TOE/Core/Ref.h>
#include <TOE/Graphics/Model.h>

namespace TOE
{
	enum class PrimitiveType
	{
		Model,
		Quad,
		Cube
	};

	class Primitives
	{
	public:
		static Ref<VAO> GetQuadVAO();
		static Ref<EBO> GetQuadEBO();
		static Ref<Model> GetQuadModel();

		static Ref<VAO> GetCubeVAO();
		static Ref<EBO> GetCubeEBO();
		static Ref<Model> GetCubeModel();
	};
}