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

		void OnUpdate(double timestep, bool viewportFocused);

		glm::mat4 GetProjection();
		glm::mat4 GetView();

	private:
		void Rotate(float deltaX, float deltaY);
		void Zoom(float distance);
		void Translate(float deltaX, float deltaY);
		glm::vec3 ToCartesian();

		void OnMouseButtonDown(MouseButtonDownEvent* event);
		void OnMouseButtonUp(MouseButtonUpEvent* event);
		void OnMouseMoved(MouseMovedEvent* event);
		void OnKeyDown(KeyDownEvent* event);
		void OnKeyUp(KeyUpEvent* event);

		float m_Pitch = 0.0f, m_Yaw = 0.0f;

		glm::vec3 m_Up;
		glm::vec3 m_Right;

		float m_FOV = 45.0f;
		float m_Near = 0.1f;
		float m_Far = 100.0f;
		unsigned int m_ViewportWidth = 1280;
		unsigned int m_ViewportHeight = 720;
	};
}