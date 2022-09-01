#pragma once
#include <TOE/Event/Events.h>

#include <entt/entt.hpp>

namespace TOE
{
	class Entity;

	class Scene
	{
	public:
		Entity CreateEntity(std::string tag = "");
		void DeleteEntity(Entity entity);
		void Update(double timestep);

	private:
		void OnWindowResizedEvent(WindowResizedEvent* event);

		entt::registry m_Registry;

		friend class Entity;
	};
}