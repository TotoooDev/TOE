#pragma once

#include <TOE/Event/Event.h>

namespace TOE
{
	struct WindowClosedEvent : public Event
	{
		WindowClosedEvent()
		{
			m_Name = "Window Closed Event";
		}
	};

	struct WindowMovedEvent : public Event
	{
		WindowMovedEvent(int xPos, int yPos)
			: x(xPos), y(yPos)
		{
			m_Name = "Window Moved Event";
		}

		int x, y;
	};

	struct WindowResizedEvent : public Event
	{
		WindowResizedEvent(unsigned int width, unsigned int height)
			: Width(width), Height(height)
		{
			m_Name = "Window Resized Event";
		}

		unsigned int Width, Height;
	};
}