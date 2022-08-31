#pragma once

#include <TOE/Event/Events.h>
#include <glm/glm.hpp>

namespace TOE
{
	class PerspectiveCamera
	{
	public:
		PerspectiveCamera(float yaw = -90.0f,
						  float pitch = 0.0f,
						  float roll = 0.0f,
						  float fov = 45.0f,
						  float viewPortWidth = 1280.0f,
						  float viewportHeight = 720.0f,
						  float near = 0.1f,
						  float far = 100.0f);

		glm::vec3 Front;
		glm::vec3 Up;
		glm::vec3 Right;

		float Yaw;
		float Pitch;
		float Roll;

		float FOV;
		float Near;
		float Far;

		float ViewportWidth;
		float ViewportHeight;

		glm::mat4 GetProjectionMatrix();
		void UpdateCameraVectors();

	private:
		void OnWindowResizedEvent(WindowResizedEvent* event);
	};
}