#pragma once

#include <TOE/Core/Ref.h>
#include <TOE/Editor/EditorCamera.h>

namespace TOE
{
	class SettingsPanel
	{
	public:
		void Init(Ref<EditorCamera> cam);
		void Draw(bool* isOpen);

	private:
		Ref<EditorCamera> m_Camera;
	};
}