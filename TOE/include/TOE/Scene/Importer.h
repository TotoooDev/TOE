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
		Importer(const std::string& path, const std::string& materialDir = "");
		Ref<Model> LoadModelFromFile();
		std::pair<Ref<Model>, std::vector<Material>> LoadModelAndMaterialFromFile(bool flipUVs = false);

	private:
		void ProcessNode(aiNode* node, const aiScene* scene);
		Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
		void ProcessTextures(aiMaterial* mat, const aiScene* scene);
		Ref<Texture2D> ProcessTexture(aiMaterial* mat, TextureType type);

		std::vector<Mesh> m_Meshes;
		std::vector<Material> m_Materials;
		std::vector<Material> m_LoadedMaterials;

		std::string m_Path;
		std::string m_MaterialDir;
	};
}