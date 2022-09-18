#include "pch.h"
#include <TOE/Scene/Importer.h>

namespace TOE
{
	Importer::Importer(const std::string& path, const std::string& materialDir)
		: m_Path(path), m_MaterialDir(materialDir)
	{

	}

	Ref<Model> Importer::LoadModelFromFile()
	{
		// Yes I know this loads the material too but I don't care lol
		return LoadModelAndMaterialFromFile().first;
	}

	std::pair<Ref<Model>, std::vector<Material>> Importer::LoadModelAndMaterialFromFile(bool flipUVs)
	{
		if (m_MaterialDir.empty())
		{
			m_MaterialDir = m_Path.substr(0, m_Path.find_last_of("\\")) + '\\';
		}

		Assimp::Importer importer;
		aiPostProcessSteps flags = aiProcess_Triangulate;
		if (flipUVs)
			flags = (aiPostProcessSteps)(flags | aiProcess_FlipUVs);

		const aiScene* scene = importer.ReadFile(m_Path, flags);
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			spdlog::error("Failed to load model at {}!", m_Path);
			return { };
		}

		ProcessNode(scene->mRootNode, scene);
		auto model = CreateRef<Model>(m_Meshes, m_Path);
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

		return Mesh(vertices, indices, mesh->mMaterialIndex);
	}

	void Importer::ProcessTextures(aiMaterial* mat, const aiScene* scene)
	{
		Material material;

		// Don't process the material if it is already loaded
		for (unsigned int j = 0; j < m_LoadedMaterials.size(); j++)
		{
			if (m_LoadedMaterials[j].Name == mat->GetName().C_Str())
			{
				return;
			}
		}

		auto diffuse = ProcessTexture(mat, TextureType::Diffuse);
		auto specular = ProcessTexture(mat, TextureType::Specular);
		auto normal = ProcessTexture(mat, TextureType::Normal);

		if (!diffuse && !specular && !normal)
			return;

		material.Diffuse = diffuse;
		material.Specular = specular;
		material.Normal = normal;
		material.Name = mat->GetName().C_Str();
		aiGetMaterialFloat(mat, AI_MATKEY_SHININESS, &material.Shininess);
		m_LoadedMaterials.push_back(material); // add to loaded materials

		m_Materials.push_back(material);
	}

	Ref<Texture2D> Importer::ProcessTexture(aiMaterial* mat, TextureType type)
	{
		// Diffuse texture
		aiString str;
		aiTextureType assimpType;
		switch (type)
		{
		case TextureType::Specular:
			assimpType = aiTextureType_SPECULAR;
			break;

		case TextureType::Normal:
			assimpType = aiTextureType_HEIGHT;
			break;

		case TextureType::Diffuse:
		default:
			assimpType = aiTextureType_DIFFUSE;
			break;
		}
		mat->GetTexture(assimpType, 0, &str);

		if (str.length == 0)
			return {};

		std::string texturePath = str.C_Str();
		// Only keep the name of the texture file
		auto found = texturePath.find_last_of("/\\");
		if (found != std::string::npos)
			texturePath = texturePath.substr(found);
		// Remove all / and \ characters
		texturePath.erase(std::remove(texturePath.begin(), texturePath.end(), '/'), texturePath.end());
		texturePath.erase(std::remove(texturePath.begin(), texturePath.end(), '\\'), texturePath.end());

		Ref<Texture2D> texture = CreateRef<Texture2D>();

		texture->CreateFromFile(m_MaterialDir + texturePath);
		texture->Type = type;

		return texture;
	}
}