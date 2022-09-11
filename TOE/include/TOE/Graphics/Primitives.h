#pragma once

#include <TOE/Core/Ref.h>
#include <TOE/Graphics/VertexObjects.h>

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
		static Ref<VAO> GetQuadVAO()
		{
			Ref<VAO> vao = CreateRef<VAO>();

			std::vector<float> vertices =
			{
				// positions        // texture coords
				 0.5f,  0.5f, 0.0f, 1.0f, 1.0f, // top right
				 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // bottom right
				-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // bottom left
				-0.5f,  0.5f, 0.0f, 0.0f, 1.0f  // top left 
			};
			VertexLayout layout;
			layout.AddAttribute(Type::Float, 3); // Pos
			layout.AddAttribute(Type::Float, 2); // Tex Coords

			vao->SetData(vertices, layout);
			return vao;
		}
		static Ref<EBO> GetQuadEBO()
		{
			Ref<EBO> ebo = CreateRef<EBO>();
			
			std::vector<unsigned int> indices =
			{
				0, 1, 3, // first triangle
				1, 2, 3  // second triangle
			};
			ebo->SetData(indices);
			return ebo;
		}
	};
}