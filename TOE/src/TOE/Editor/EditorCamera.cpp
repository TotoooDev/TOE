#include <TOE/Editor/EditorCamera.h>
#include <glm/gtc/matrix_transform.hpp>

namespace TOE
{
	EditorCamera::EditorCamera()
	{

	}

	void EditorCamera::OnUpdate(double timestep, bool viewportFocused)
	{
		if (viewportFocused)
		{
			// From https://learnopengl.com/Getting-started/Camera
			// calculate the new Front vector
			glm::vec3 front;
			front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
			front.y = sin(glm::radians(m_Pitch));
			front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
			Target = glm::normalize(front);
			// also re-calculate the Right and Up vector
			m_Right = glm::normalize(glm::cross(Target, glm::vec3(0.0f, 1.0f, 0.0f)));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
			m_Up = glm::normalize(glm::cross(m_Right, Target));
		}
	}

	glm::mat4 EditorCamera::GetProjection()
	{
		return glm::perspective(m_FOV, (float)m_ViewportWidth / (float)m_ViewportHeight, m_Near, m_Far);
	}

	glm::mat4 EditorCamera::GetView()
	{
		return glm::lookAt(Target + ToCartesian(), Target, m_Up);
	}

	void EditorCamera::Rotate(float deltaX, float deltaY)
	{
		m_Yaw += deltaX;
		m_Pitch += deltaY;
	}

	void EditorCamera::Zoom(float distance)
	{
		Distance += distance;
	}

	void EditorCamera::Translate(float deltaX, float deltaY)
	{
		glm::vec3 look = glm::normalize(ToCartesian());
		glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);

		glm::vec3 right = glm::cross(look, worldUp);
		glm::vec3 up = glm::cross(look, right);

		Target = Target + (right * deltaX) + (up * deltaY);
	}

	glm::vec3 EditorCamera::ToCartesian()
	{
		float x = Distance * sinf(m_Pitch) * sinf(m_Yaw);
		float y = Distance * cosf(m_Pitch);
		float z = Distance * sinf(m_Pitch) * cosf(m_Yaw);

		return glm::vec3(x, y, z);
	}

	void EditorCamera::OnMouseButtonDown(MouseButtonDownEvent* event)
	{
		
	}
	void EditorCamera::OnMouseButtonUp(MouseButtonUpEvent* event)
	{

	}
	void EditorCamera::OnMouseMoved(MouseMovedEvent* event)
	{

	}
	void EditorCamera::OnKeyDown(KeyDownEvent* event)
	{

	}
	void EditorCamera::OnKeyUp(KeyUpEvent* event)
	{

	}
	// Todo mouse scroll event
}