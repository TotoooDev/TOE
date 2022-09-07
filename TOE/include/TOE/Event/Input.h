#pragma once

#include <TOE/Core/Application.h>
#include <TOE/Event/Keycode.h>

namespace TOE
{
	namespace Input
	{
		bool Key(int key)
		{
			return Application::Get().GetWindow().GetKey(key) == TOE_PRESS;
		}

		bool Mouse(int button)
		{
			return Application::Get().GetWindow().GetMouseButton(button) == TOE_PRESS;
		}
	};
}