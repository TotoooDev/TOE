#pragma once

#include <glm/glm.hpp>

namespace TOE
{
	class Camera
	{
	public:
		Camera(glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f),
			float yaw = -90.0f,
			float pitch = 0.0f,
			float roll = 0.0f,
			glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f),
			float fov = 45.0f,
			float aspectRatio = 16.0f / 9.0f,
			float Near = 0.1f,
			float Far = 100.0f);

		glm::vec3 Pos;
		glm::vec3 Front;
		glm::vec3 Up;
		glm::vec3 Right;
		glm::vec3 WorldUp;

		float Yaw;
		float Pitch;
		float Roll;

		float FOV;
		float AspectRatio;
		float Near;
		float Far;

		bool Render = true;

		glm::mat4 GetViewMatrix();
		glm::mat4 GetProjectionMatrix();
		void UpdateCameraVectors();
	};
}