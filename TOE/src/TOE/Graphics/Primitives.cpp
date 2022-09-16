#include "pch.h"
#include <TOE/Graphics/Primitives.h>

namespace TOE
{
	Ref<VAO> Primitives::GetQuadVAO()
	{
		Ref<VAO> vao = CreateRef<VAO>();

		std::vector<float> vertices =
		{
			// positions        // normals        // texture coords
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, // top right
			 0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, // bottom right
			-0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, // bottom left
			-0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f  // top left 
		};
		VertexLayout layout;
		layout.AddAttribute(Type::Float, 3); // Pos
		layout.AddAttribute(Type::Float, 3); // Normals
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
	Ref<Model> Primitives::GetQuadModel()
	{
		Mesh mesh(GetQuadVAO(), GetQuadEBO());
		std::vector<Mesh> meshes =
		{
			mesh
		};
		Ref<Model> model = CreateRef<Model>(meshes);
		return model;
	}
}