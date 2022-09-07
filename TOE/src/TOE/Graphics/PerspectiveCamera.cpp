#include <TOE/Graphics/PerspectiveCamera.h>
#include <TOE/Core/Application.h>
#include <TOE/Debug/Instrumentor.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace TOE
{
	PerspectiveCamera::PerspectiveCamera(float yaw, float pitch, float roll, float fov, float viewportWidth, float viewportHeight, float near, float far)
		: Yaw(yaw), Pitch(pitch), Roll(roll), FOV(fov), ViewportWidth(viewportWidth), ViewportHeight(viewportHeight), Near(near), Far(far)
	{
		UpdateCameraVectors();
	}

	glm::mat4 PerspectiveCamera::GetProjectionMatrix()
	{
		return glm::perspective(FOV, ViewportWidth / ViewportHeight, Near, Far);
	}

	void PerspectiveCamera::UpdateCameraVectors()
	{
		TOE_PROFILE_FUNCTION();

		// From https://learnopengl.com/Getting-started/Camera
		// calculate the new Front vector
		glm::vec3 front;
		front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		front.y = sin(glm::radians(Pitch));
		front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		Front = glm::normalize(front);
		// also re-calculate the Right and Up vector
		Right = glm::normalize(glm::cross(Front, glm::vec3(0.0f, 1.0f, 0.0f)));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		Up = glm::normalize(glm::cross(Right, Front));

		glm::mat4 rollMat = glm::rotate(glm::mat4(1.0f), glm::radians(Roll), Front);
		Up = glm::mat3(rollMat) * Up;
	}
}