#pragma once

#include <TOE/Core/Ref.h>
#include <TOE/Graphics/Mesh.h>

namespace TOE
{
	enum class PrimitiveType
	{
		Model,
		Quad
	};

	class Primitives
	{
	public:
		static Ref<VAO> GetQuadVAO();
		static Ref<EBO> GetQuadEBO();
		static Ref<Model> GetQuadModel();
	};
}