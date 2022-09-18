#include "pch.h"
#include <TOE/Graphics/Mesh.h>

namespace TOE
{
	Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, unsigned int materialIndex)
	{
		m_VAO = CreateRef<VAO>();
		m_EBO = CreateRef<EBO>();

		VertexLayout layout;
		layout.AddAttribute(Type::Float, 3); // Position
		layout.AddAttribute(Type::Float, 3); // Normal
		layout.AddAttribute(Type::Float, 2); // Texture coordinates
		
		m_VAO->SetData(vertices, layout);

		m_EBO->SetData(indices);
	}
	Mesh::Mesh(Ref<VAO> vao, Ref<EBO> ebo, unsigned int materialIndex)
		: m_VAO(vao), m_EBO(ebo), m_MaterialIndex(materialIndex)
	{

	}
}