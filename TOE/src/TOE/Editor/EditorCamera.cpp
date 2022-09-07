#include <TOE/Editor/EditorCamera.h>
#include <TOE/Event/Input.h>
#include <TOE/Core/Application.h>
#include <glm/gtc/matrix_transform.hpp>

namespace TOE
{
	EditorCamera::EditorCamera()
	{
		TOE::Application::Get().EventBus.Subscribe(this, &EditorCamera::OnMouseMoved);
		TOE::Application::Get().EventBus.Subscribe(this, &EditorCamera::OnMouseScroll);
		Rotate(0.0f, glm::pi<float>() / 2.0f);
	}

	void EditorCamera::OnUpdate(double timestep, bool viewportFocused)
	{
		m_ViewportHover = viewportFocused;
		if (m_ViewportHover)
		{
			m_Timestep = timestep;
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

	void EditorCamera::OnViewportResize(unsigned int width, unsigned int height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;
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
		if (m_Pitch > glm::pi<float>())
			m_Pitch = glm::pi<float>();
		if (m_Pitch < 0)
			m_Pitch = 0;
	}

	void EditorCamera::Zoom(float distance)
	{
		Distance += distance;
	}

	void EditorCamera::Translate(float deltaX, float deltaY)
	{
		glm::vec3 look = glm::normalize(Target - ToCartesian());
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

	void EditorCamera::OnMouseMoved(MouseMovedEvent* event)
	{
		if (!m_ViewportHover)
			return;
		if (Input::Mouse(TOE_MOUSE_BUTTON_3))
		{
			if (Input::Key(TOE_KEY_LEFT_SHIFT))
			{
				float deltaX = m_LastMouse.x - event->x;
				float deltaY = m_LastMouse.y - event->y;
				Translate(deltaX, deltaY);
			}
			else
			{
				float deltaX = m_LastMouse.x - event->x;
				float deltaY = m_LastMouse.y - event->y;
				Rotate(deltaX * (float)m_Timestep * (Sensibility * 0.01f), deltaY * (float)m_Timestep * (Sensibility * 0.01f));
			}
			m_LastMouse = glm::vec2((float)event->x, (float)event->y);
		}
		else
			m_LastMouse = glm::vec2((float)event->x, (float)event->y);
	}
	void EditorCamera::OnMouseScroll(MouseScrolledEvent* event)
	{
		if (m_ViewportHover)
			Zoom(-event->yOffset);
	}
}