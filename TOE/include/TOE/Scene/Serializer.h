#pragma once

#include <TOE/Core/Ref.h>
#include <TOE/Scene/Scene.h>

#include <nlohmann/json.hpp>

namespace TOE
{
	class SceneSerializer
	{
	public:
		SceneSerializer(Ref<Scene> scene);

		void Serialize(const std::string& path);
		void Deserialize(const std::string& path);

	private:
		void SerializeEntity(Entity entity, nlohmann::json& entityJson);
		void DeserializeEntity(Entity& entity, nlohmann::json& entityJson);
		void SerializeVec3(const glm::vec3& vec, nlohmann::json& json);
		glm::vec3 DeserializeVec3(nlohmann::json& json);

		Ref<Scene> m_Scene;
	};
}