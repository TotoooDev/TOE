#pragma once

#include <TOE/Core/Ref.h>
#include <TOE/Graphics/VertexObjects.h>

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

		static Ref<VAO> GetCubeVAO();
		static Ref<EBO> GetCubeEBO();
	};
}