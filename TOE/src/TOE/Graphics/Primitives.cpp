#include "pch.h"
#include <TOE/Graphics/Primitives.h>

namespace TOE
{
	Ref<VAO> Primitives::GetQuadVAO()
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
	Ref<EBO> Primitives::GetQuadEBO()
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
	// Ref<Model> Primitives::GetQuadModel()
	// {
	// 	std::vector<Mesh> meshes;
	// 	Mesh mesh(GetQuadVAO(), GetQuadEBO());
	// 	meshes.push_back(mesh);
	// 	return Ref<Model>();
	// }

	Ref<VAO> Primitives::GetCubeVAO()
	{
		Ref<VAO> vao = CreateRef<VAO>();

		std::vector<float> vertices =
		{
			// positions        // texture coords
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
			 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
		};
		VertexLayout layout;
		layout.AddAttribute(Type::Float, 3); // Pos
		layout.AddAttribute(Type::Float, 2); // Tex Coords

		vao->SetData(vertices, layout);
		return vao;
	}
	Ref<EBO> Primitives::GetCubeEBO()
	{
		Ref<EBO> ebo = CreateRef<EBO>();

		std::vector<unsigned int> indices;
		for (unsigned int i = 0; i < 36; i++)
		{
			indices.push_back(i);
		}

		ebo->SetData(indices);
		return ebo;
	}
}