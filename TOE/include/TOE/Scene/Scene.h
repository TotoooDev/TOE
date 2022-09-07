#pragma once
#include <TOE/Event/Events.h>

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
		void Update(double timestep);

		void OnViewportResize(unsigned int width, unsigned int height);

	private:
		void OnWindowResized(WindowResizedEvent* event);
		void OnMouseButtonDown(MouseButtonDownEvent* event);
		void OnMouseButtonUp(MouseButtonUpEvent* event);
		void OnMouseMoved(MouseMovedEvent* event);

		entt::registry m_Registry;
		bool m_MouseButtonDown = false;
		double m_Timestep;
		double m_LastMouseX, m_LastMouseY;
		double m_CamSensibility = 5.0f;

		friend class Entity;
		friend class ScenePanel;
	};
}