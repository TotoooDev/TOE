#pragma once

#include <TOE/Graphics/Model.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>

namespace TOE
{
	class Importer
	{
	public:
		Ref<Model> LoadModelFromFile(const std::string& path);

	private:
		void ProcessNode(aiNode* node, const aiScene* scene);
		Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);

		std::vector<Mesh> m_Meshes;
		std::string m_Directory;
	};
}