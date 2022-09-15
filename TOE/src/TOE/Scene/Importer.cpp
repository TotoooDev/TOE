#include "pch.h"
#include <TOE/Scene/Importer.h>

namespace TOE
{
	std::pair<Ref<Model>, std::vector<Material>> Importer::LoadModelFromFile(const std::string& path)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			spdlog::error("Failed to load model at {}!", path);
			return { };
		}
		m_Directory = path.substr(0, path.find_last_of('/'));

		ProcessNode(scene->mRootNode, scene);
		auto model = CreateRef<Model>(m_Meshes);
		return std::pair<Ref<Model>, std::vector<Material>>(model, m_Materials);
	}

	void Importer::ProcessNode(aiNode* node, const aiScene* scene)
	{
		std::vector<Mesh> meshes;
		// Process all the meshes if any
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			m_Meshes.push_back(ProcessMesh(mesh, scene));
		}
		// Process other nodes
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			ProcessNode(node->mChildren[i], scene);
		}
	}

	Mesh Importer::ProcessMesh(aiMesh* mesh, const aiScene* scene)
	{
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;

		// Process vertices
		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex;

			glm::vec3 pos;
			pos.x = mesh->mVertices[i].x;
			pos.y = mesh->mVertices[i].y;
			pos.z = mesh->mVertices[i].z;
			vertex.Pos = pos;

			if (mesh->mNormals)
			{
				glm::vec3 normal;
				normal.x = mesh->mNormals[i].x;
				normal.y = mesh->mNormals[i].y;
				normal.z = mesh->mNormals[i].z;
				vertex.Normal = normal;
			}
			else
			{
				vertex.Normal = glm::vec3(0.0f);
			}

			if (mesh->mTextureCoords[0])
			{
				glm::vec2 texCoords;
				texCoords.x = mesh->mTextureCoords[0][i].x;
				texCoords.y = mesh->mTextureCoords[0][i].y;
				vertex.TextureCoords = texCoords;
			}
			else
			{
				vertex.TextureCoords = glm::vec2(0.0f);
			}

			vertices.push_back(vertex);
		}

		// Process indices
		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; j++)
			{
				indices.push_back(face.mIndices[j]);
			}
		}

		// Process materials
		if (mesh->mMaterialIndex >= 0)
		{
			aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];
			ProcessTextures(mat, scene);
		}

		return Mesh(vertices, indices);
	}

	void Importer::ProcessTextures(aiMaterial* mat, const aiScene* scene)
	{
		Material material;

		// Process diffuse texture
		aiString str;
		mat->GetTexture(aiTextureType_DIFFUSE, 0, &str);
		bool skip = false;
		for (unsigned int j = 0; j < m_LoadedTextures.size(); j++)
		{
			if (std::strcmp(m_LoadedTextures[j].Path.data(), str.C_Str()) == 0)
			{
				material.Diffuse = CreateRef<Texture2D>(m_LoadedTextures[j]);
				skip = true;
				break;
			}
		}
		if (!skip)
		{   // if texture hasn't been loaded already, load it
			Texture2D texture;
			std::string path = str.C_Str();
			if (path != "")
			{
				auto found = path.find_last_of("/\\");
				path = path.substr(found);
				texture.CreateFromFile("textures/" + path);
				texture.Type = TextureType::Diffuse;
				texture.Path = str.C_Str();
				m_LoadedTextures.push_back(texture); // add to loaded textures
				material.Diffuse = CreateRef<Texture2D>(texture);
			}
		}

		m_Materials.push_back(material);
	}
}