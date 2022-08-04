#pragma once

#include <TOE/Event/Event.h>

namespace TOE
{
	struct MouseMovedEvent : public Event
	{
		MouseMovedEvent(double xPos, double yPos)
			: x(xPos), y(yPos)
		{
			m_Name = "Mouse Moved Event";
		}

		double x, y;
	};

	struct MouseButtonDownEvent : public Event
	{
		MouseButtonDownEvent(int button)
			: Button(button)
		{
			m_Name = "Mouse Button Down Event";
		}

		int Button;
	};

	struct MouseButtonUpEvent : public Event
	{
		MouseButtonUpEvent(int button)
			: Button(button)
		{
			m_Name = "Mouse Button Up Event";
		}

		int Button;
	};
}