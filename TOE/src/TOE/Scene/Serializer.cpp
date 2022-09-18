#include "pch.h"
#include <TOE/Scene/Serializer.h>
#include <TOE/Scene/Entity.h>
#include <TOE/Scene/Components.h>
#include <TOE/Scene/Importer.h>
#include <TOE/Graphics/Primitives.h>

#include <fstream>

namespace TOE
{
	SceneSerializer::SceneSerializer(Ref<Scene> scene)
		: m_Scene(scene)
	{

	}

	void SceneSerializer::Serialize(const std::string& path)
	{
		nlohmann::json json;

		json["scene"]["name"] = m_Scene->Name;

		int num = 0;
		auto& registry = m_Scene->m_Registry;
		registry.each([&](auto entity)
			{
				Entity ent(entity, m_Scene.get());
				if (!ent.IsValid())
					return;

				SerializeEntity(ent, json["scene"]["entities"][num]);
				num++;
			});

		std::ofstream file(path);
		file << std::setw(4) << json;
	}

	void SceneSerializer::Deserialize(const std::string& path)
	{
		std::ifstream file;
		file.open(path);
		nlohmann::json json = nlohmann::json::parse(file);
		file.close();

		if (!json.contains("scene"))
		{
			spdlog::error("{0} is not a valid TOE scene!", path);
			return;
		}

		m_Scene->Name = json["scene"]["name"];

		unsigned int numEntities = json["scene"]["entities"].size();
		for (unsigned int i = 0; i < numEntities; i++)
		{
			auto ent = m_Scene->CreateEntity();
			DeserializeEntity(ent, json["scene"]["entities"][i]);
		}
	}

	void SceneSerializer::SerializeEntity(Entity entity, nlohmann::json& entityJson)
	{
		if (entity.HasComponent<TagComponent>())
		{
			auto& tagComp = entity.GetComponent<TagComponent>();
			entityJson["tag"]["tag"] = tagComp.Tag;
		}

		if (entity.HasComponent<TransformComponent>())
		{
			auto& transformComp = entity.GetComponent<TransformComponent>();
			SerializeVec3(transformComp.Translation, entityJson["transform"]["translation"]);
			SerializeVec3(transformComp.Rotation, entityJson["transform"]["rotation"]);
			SerializeVec3(transformComp.Scale, entityJson["transform"]["scale"]);
		}

		// if (entity.HasComponent<MaterialComponent>())
		// {
		// 	auto& materialComp = entity.GetComponent<MaterialComponent>();
		// 	SerializeVec3(materialComp.AlbedoColor, entityJson["material"]["albedo_color"]);
		// 	if (materialComp.Albedo)
		// 		entityJson["material"]["albedo"] = materialComp.Albedo->GetPath();
		// 	else
		// 		entityJson["material"]["albedo"] = "";
		// 	entityJson["material"]["use_color"] = materialComp.UseColor;
		// }

		if (entity.HasComponent<MeshComponent>())
		{
			auto& meshComp = entity.GetComponent<MeshComponent>();
			entityJson["mesh"]["path"] = meshComp.Model->GetPath();
			entityJson["mesh"]["render"] = meshComp.Render;
		}

		if (entity.HasComponent<LightComponent>())
		{
			auto& lightComp = entity.GetComponent<LightComponent>();
			entityJson["light"]["type"] = lightComp.Light.Type;
			SerializeVec3(lightComp.Light.Ambient, entityJson["light"]["ambient"]);
			SerializeVec3(lightComp.Light.Diffuse, entityJson["light"]["diffuse"]);
			SerializeVec3(lightComp.Light.Specular, entityJson["light"]["specular"]);
			entityJson["light"]["constant"] = lightComp.Light.Constant;
			entityJson["light"]["linear"] = lightComp.Light.Linear;
			entityJson["light"]["quadratic"] = lightComp.Light.Quadratic;
			entityJson["light"]["cut_off"] = lightComp.Light.CutOff;
			entityJson["light"]["outer_cut_off"] = lightComp.Light.OuterCutOff;
		}
	}

	void SceneSerializer::DeserializeEntity(Entity& entity, nlohmann::json& entityJson)
	{
		// Tag and transform components are created along with the entity so we dont need to create them
		// We should always have a tag component attached to an entity
		if (entityJson.contains("tag"))
		{
			entity.GetComponent<TagComponent>().Tag = entityJson["tag"]["tag"];
		}
		else
		{
			entity.GetComponent<TagComponent>().Tag = "Unnamed Entity";
		}

		if (entityJson.contains("transform"))
		{
			auto& transformComp = entity.GetComponent<TransformComponent>();
			transformComp.Translation = DeserializeVec3(entityJson["transform"]["translation"]);
			transformComp.Rotation = DeserializeVec3(entityJson["transform"]["rotation"]);
			transformComp.Scale = DeserializeVec3(entityJson["transform"]["scale"]);
		}

		// if (entityJson.contains("material"))
		// {
		// 	auto& materialComp = entity.AddComponent<MaterialComponent>();
		// 	materialComp.AlbedoColor = DeserializeVec3(entityJson["material"]["albedo_color"]);
		// 	if (entityJson["material"]["albedo"] != "")
		// 	{
		// 		auto texture = CreateRef<Texture2D>();
		// 		texture->CreateFromFile(entityJson["material"]["albedo"]);
		// 		materialComp.Albedo = texture;
		// 	}
		// 	materialComp.UseColor = entityJson["material"]["use_color"];
		// }

		if (entityJson.contains("mesh"))
		{
			std::string path = entityJson["mesh"]["path"];
			Importer importer(path);
			auto&& [model, materials] = importer.LoadModelAndMaterialFromFile();

			auto& meshComp = entity.AddComponent<MeshComponent>(model);
			if (materials.size() > 0)
				auto& materialComp = entity.AddComponent<MaterialComponent>(materials);

			meshComp.Render = entityJson["mesh"]["render"];
 		}

		if (entityJson.contains("light"))
		{
			Light light =
			{
				entityJson["light"]["type"],
				DeserializeVec3(entityJson["light"]["ambient"]),
				DeserializeVec3(entityJson["light"]["diffuse"]),
				DeserializeVec3(entityJson["light"]["specular"]),
				entityJson["light"]["constant"],
				entityJson["light"]["linear"],
				entityJson["light"]["quadratic"],
				entityJson["light"]["cut_off"],
				entityJson["light"]["outer_cut_off"],
			};
			auto& comp = entity.AddComponent<LightComponent>();
			comp.Light = light;
		}
	}

	void SceneSerializer::SerializeVec3(const glm::vec3& vec, nlohmann::json& json)
	{
		json["x"] = vec.x;
		json["y"] = vec.y;
		json["z"] = vec.z;
	}

	glm::vec3 SceneSerializer::DeserializeVec3(nlohmann::json& json)
	{
		return glm::vec3(json["x"], json["y"], json["z"]);
	}
}