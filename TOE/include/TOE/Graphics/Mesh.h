#pragma once

#include <TOE/Core/Ref.h>
#include <TOE/Graphics/VertexObjects.h>
#include <TOE/Graphics/Texture.h>

#include <glm/glm.hpp>
#include <vector>

namespace TOE
{
	struct Vertex
	{
		glm::vec3 Pos;
		glm::vec3 Normal;
		glm::vec2 TextureCoords;
	};

	class Mesh
	{
	public:
		Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);

		std::vector<Vertex> GetVertices() const { return m_Vertices; }
		std::vector<unsigned int> GetIndices() const { return m_Indices; }

		Ref<VAO> GetVAO() const { return m_VAO; }
		Ref<EBO> GetEBO() const { return m_EBO; }

	private:
		void SetupMesh();

		std::vector<Vertex> m_Vertices;
		std::vector<unsigned int> m_Indices;

		Ref<VAO> m_VAO;
		Ref<EBO> m_EBO;
	};
}