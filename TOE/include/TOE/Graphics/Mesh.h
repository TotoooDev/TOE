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
		Mesh(Ref<VAO> vao, Ref<EBO> ebo);

		Ref<VAO> GetVAO() const { return m_VAO; }
		Ref<EBO> GetEBO() const { return m_EBO; }

	private:
		Ref<VAO> m_VAO;
		Ref<EBO> m_EBO;
	};

	class Model
	{
	public:
		Model(std::vector<Mesh> meshes, const std::string& path = "")
			: m_Meshes(meshes), m_Path(path) {}

		std::vector<Mesh> GetMeshes() { return m_Meshes; }
		std::string GetPath() { return m_Path; }

	private:
		std::vector<Mesh> m_Meshes;
		std::string m_Path;
	};
}