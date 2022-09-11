#pragma once
#include <TOE/Core/Ref.h>
#include <TOE/Event/Events.h>
#include <TOE/Editor/EditorCamera.h>

#include <entt/entt.hpp>

namespace TOE
{
	class Entity;

	class Scene
	{
	public:
		Scene() = default;
		Scene(const Scene&) = default;

		Entity CreateEntity(const std::string& tag = "");
		void DeleteEntity(Entity entity);
		void UpdateEditor(double timestep, Ref<EditorCamera> camera);
		void UpdateRuntime(double timestep);

		void OnViewportResize(unsigned int width, unsigned int height);

		std::string Name = "New Scene";

	private:
		void OnWindowResized(WindowResizedEvent* event);

		entt::registry m_Registry;

		friend class Entity;
		friend class ScenePanel;
		friend class SceneSerializer;
	};
}