#include <TOE/CORE/Camera.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace TOE
{
	Camera::Camera(glm::vec3 pos, float yaw, float pitch, float roll, glm::vec3 worldUp, float fov, float aspectRatio, float near, float far)
		: Pos(pos), Yaw(yaw), Pitch(pitch), Roll(roll), WorldUp(worldUp), FOV(fov), AspectRatio(aspectRatio), Near(near), Far(far)
	{
		UpdateCameraVectors();
	}

	glm::mat4 Camera::GetViewMatrix()
	{
		UpdateCameraVectors();
		return glm::lookAt(Pos, Pos + Front, Up);
	}

	glm::mat4 Camera::GetProjectionMatrix()
	{
		return glm::perspective(FOV, AspectRatio, Near, Far);
	}

	void Camera::UpdateCameraVectors()
	{
		// From https://learnopengl.com/Getting-started/Camera
		// calculate the new Front vector
		glm::vec3 front;
		front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		front.y = sin(glm::radians(Pitch));
		front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		Front = glm::normalize(front);
		// also re-calculate the Right and Up vector
		Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		Up = glm::normalize(glm::cross(Right, Front));

		glm::mat4 rollMat = glm::rotate(glm::mat4(1.0f), glm::radians(Roll), Front);
		Up = glm::mat3(rollMat) * Up;
	}
}