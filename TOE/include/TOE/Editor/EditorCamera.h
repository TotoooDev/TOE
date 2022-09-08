#pragma once

#include <TOE/Event/Events.h>
#include <glm/glm.hpp>

namespace TOE
{
	// Stolen from Hazel
	class EditorCamera
	{
	public:
		EditorCamera();

		void OnUpdate(double timestep);
		void OnViewportResize(unsigned int width, unsigned int height);

		void SetViewportFocus(bool focus) { m_ViewportFocus = focus; }

		glm::mat4 GetProjection() const { return m_Projection; }
		glm::mat4 GetView() const { return m_View; }

		float Sensibility;

	private:
		void UpdateView();
		void UpdateProjection();

		void Pan(const glm::vec2& delta);
		void Rotate(const glm::vec2& delta);
		void Zoom(float delta);

		glm::vec2 GetPanSpeed();
		float GetRotationSpeed();
		float GetZoomSpeed();

		glm::vec3 CalculatePosition();
		glm::vec3 GetForwardDirection();
		glm::vec3 GetRightDirection();
		glm::vec3 GetUpDirection();
		glm::quat GetOrientation();

		void OnMouseMoved(MouseMovedEvent* event);
		void OnMouseScrolled(MouseScrolledEvent* event);

	private:
		bool m_ViewportFocus = true;

		float m_MaxPanSpeed = 2.4f;
		float m_MaxZoomSpeed = 100.0f;
		float m_FOV = 45.0f, m_AspectRatio = 16.0f / 9.0f, m_NearClip = 0.1f, m_FarClip = 1000.0f;

		glm::mat4 m_View, m_Projection;
		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_FocalPoint = { 0.0f, 0.0f, 0.0f };

		glm::vec2 m_InitialMousePosition = { 0.0f, 0.0f };

		float m_Distance = 10.0f;
		float m_Pitch = 0.0f, m_Yaw = 0.0f;

		float m_ViewportWidth = 1280, m_ViewportHeight = 720;
	};
}