#pragma once

#include <TOE/Graphics/Mesh.h>
#include <TOE/Graphics/Material.h>

#include <utility>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>

namespace TOE
{
	class Importer
	{
	public:
		std::pair<Ref<Model>, std::vector<Material>> LoadModelFromFile(const std::string& path);

	private:
		void ProcessNode(aiNode* node, const aiScene* scene);
		Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
		void ProcessTextures(aiMaterial* mat, const aiScene* scene);

		std::vector<Mesh> m_Meshes;
		std::vector<Material> m_Materials;
		std::vector<Texture2D> m_LoadedTextures;
		std::string m_Directory;
	};
}