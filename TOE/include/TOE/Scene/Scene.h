#pragma once

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
		entt::registry m_Registry;

		friend class Entity;
	};
}