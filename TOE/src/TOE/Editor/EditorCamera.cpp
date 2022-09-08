#include <TOE/Editor/EditorCamera.h>
#include <TOE/Event/Input.h>
#include <TOE/Core/Application.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <spdlog/spdlog.h>

namespace TOE
{
	EditorCamera::EditorCamera()
	{
		Application::Get().EventBus.Subscribe(this, &EditorCamera::OnMouseMoved);
		Application::Get().EventBus.Subscribe(this, &EditorCamera::OnMouseScrolled);
	}

	void EditorCamera::OnUpdate(double timestep)
	{
		UpdateView();
	}

	void EditorCamera::OnViewportResize(unsigned int width, unsigned int height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;
		UpdateProjection();
	}

	void EditorCamera::UpdateView()
	{
		m_Position = CalculatePosition();

		glm::quat orientation = GetOrientation();
		m_View = glm::translate(glm::mat4(1.0f), m_Position) * glm::toMat4(orientation);
		m_View = glm::inverse(m_View);
	}

	void EditorCamera::UpdateProjection()
	{
		m_AspectRatio = m_ViewportWidth / m_ViewportHeight;
		m_Projection = glm::perspective(glm::radians(m_FOV), m_AspectRatio, m_NearClip, m_FarClip);
	}

	void EditorCamera::Pan(const glm::vec2& delta)
	{
		glm::vec2 speed = GetPanSpeed();
		m_FocalPoint += -GetRightDirection() * delta.x * speed.x * m_Distance;
		m_FocalPoint += GetUpDirection() * delta.y * speed.y * m_Distance;
	}

	void EditorCamera::Rotate(const glm::vec2& delta)
	{
		float yawSign = GetUpDirection().y < 0.0f ? -1.0f : 1.0f;
		m_Yaw += yawSign * delta.x * GetRotationSpeed();
		m_Pitch += delta.y * GetRotationSpeed();
	}

	void EditorCamera::Zoom(float delta)
	{
		m_Distance -= delta * GetZoomSpeed();
		if (m_Distance < 1.0f)
		{
			m_FocalPoint += GetForwardDirection();
			m_Distance = 1.0f;
		}
	}

	glm::vec2 EditorCamera::GetPanSpeed()
	{
		float x = std::min(m_ViewportWidth / 1000.0f, m_MaxPanSpeed);
		float xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;

		float y = std::min(m_ViewportHeight / 1000.0f, m_MaxPanSpeed);
		float yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

		return { xFactor, yFactor };
	}

	float EditorCamera::GetRotationSpeed()
	{
		return 0.8f;
	}

	float EditorCamera::GetZoomSpeed()
	{
		float distance = m_Distance * 0.2f;
		distance = std::max(distance, 0.0f);
		float speed = distance * distance;
		speed = std::min(speed, m_MaxZoomSpeed);
		return speed;
	}

	glm::vec3 EditorCamera::CalculatePosition()
	{
		return m_FocalPoint - GetForwardDirection() * m_Distance;
	}
	glm::vec3 EditorCamera::GetForwardDirection()
	{
		return glm::rotate(GetOrientation(), glm::vec3(0.0f, 0.0f, -1.0f));
	}
	glm::vec3 EditorCamera::GetRightDirection()
	{
		return glm::rotate(GetOrientation(), glm::vec3(1.0f, 0.0f, 0.0f));
	}
	glm::vec3 EditorCamera::GetUpDirection()
	{
		return glm::rotate(GetOrientation(), glm::vec3(0.0f, 1.0f, 0.0f));
	}
	glm::quat EditorCamera::GetOrientation()
	{
		return glm::quat(glm::vec3(-m_Pitch, -m_Yaw, 0.0f));
	}

	void EditorCamera::OnMouseMoved(MouseMovedEvent* event)
	{
		if (m_ViewportFocus)
		{
			if (Input::Mouse(TOE_MOUSE_BUTTON_3))
			{
				glm::vec2 mouse = glm::vec2(event->x, event->y);
				glm::vec2 delta = (mouse - m_InitialMousePosition) * (Sensibility * 0.0005f);
				m_InitialMousePosition = mouse;

				if (Input::Key(TOE_KEY_LEFT_SHIFT))
				{
					Pan(delta);
				}
				else
				{
					Rotate(delta);
				}
			}
			m_InitialMousePosition = glm::vec2(event->x, event->y);
		}
	}
	void EditorCamera::OnMouseScrolled(MouseScrolledEvent* event)
	{
		if (m_ViewportFocus)
		{
			float delta = event->yOffset * 0.1f;
			Zoom(delta);
			UpdateView();
		}
	}
}