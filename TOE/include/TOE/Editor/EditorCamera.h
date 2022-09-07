#pragma once

#include <TOE/Event/Events.h>
#include <glm/glm.hpp>

namespace TOE
{
	class EditorCamera
	{
	public:
		EditorCamera();

		glm::vec3 Target = glm::vec3(0.0f, 0.0f, 0.0f);
		float Distance = 10.0f;
		float Sensibility = 15.0f;

		void OnUpdate(double timestep, bool viewportFocused);
		void OnViewportResize(unsigned int width, unsigned int height);

		glm::mat4 GetProjection();
		glm::mat4 GetView();

	private:
		void Rotate(float deltaX, float deltaY);
		void Zoom(float distance);
		void Translate(float deltaX, float deltaY);
		glm::vec3 ToCartesian();

		void OnMouseMoved(MouseMovedEvent* event);
		void OnMouseScroll(MouseScrolledEvent* event);

		float m_Pitch = 0.0f, m_Yaw = 0.0f;

		glm::vec3 m_Up = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 m_Right = glm::vec3(1.0f, 0.0f, 0.0f);

		float m_FOV = 45.0f;
		float m_Near = 0.1f;
		float m_Far = 100.0f;
		unsigned int m_ViewportWidth = 1280;
		unsigned int m_ViewportHeight = 720;

		bool m_ViewportHover = true;
		double m_Timestep = 0.0f;

		glm::vec2 m_LastMouse = glm::vec2(0.0f, 0.0f);
	};
}