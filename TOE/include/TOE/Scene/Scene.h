#pragma once
#include <TOE/Event/Events.h>

#include <entt/entt.hpp>

namespace TOE
{
	class Entity;

	class Scene
	{
	public:
		Entity CreateEntity(const std::string& tag = "");
		void DeleteEntity(Entity entity);
		void Update(double timestep);

		void OnViewportResize(unsigned int width, unsigned int height);

	private:
		void OnWindowResizedEvent(WindowResizedEvent* event);

		entt::registry m_Registry;

		friend class Entity;
	};
}