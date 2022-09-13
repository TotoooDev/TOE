#include "pch.h"
#include <TOE/Graphics/Mesh.h>

namespace TOE
{
	Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices)
		: m_Vertices(vertices), m_Indices(indices)
	{
		m_VAO = CreateRef<VAO>();
		m_EBO = CreateRef<EBO>();

		SetupMesh();
	}

	void Mesh::SetupMesh()
	{
		VertexLayout layout;
		layout.AddAttribute(Type::Float, 3); // Position
		layout.AddAttribute(Type::Float, 3); // Normal
		layout.AddAttribute(Type::Float, 2); // Texture coordinates
		
		m_VAO->SetData(m_Vertices, layout);

		m_EBO->SetData(m_Indices);
	}
}