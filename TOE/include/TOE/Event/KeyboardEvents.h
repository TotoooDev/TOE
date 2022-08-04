#pragma once

#include <TOE/Event/Event.h>

namespace TOE
{
	struct KeyDownEvent : public Event
	{
		KeyDownEvent(int keycode)
			: Keycode(keycode)
		{
			m_Name = "Key Down Event";
		}

		int Keycode;
	};

	struct KeyUpEvent : public Event
	{
		KeyUpEvent(int keycode)
			: Keycode(keycode)
		{
			m_Name = "Key Up Event";
		}

		int Keycode;
	};
}