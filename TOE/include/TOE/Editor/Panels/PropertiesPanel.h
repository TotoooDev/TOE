#pragma once

#include <TOE/Core/Ref.h>
#include <TOE/Editor/Panels/ScenePanel.h>

namespace TOE
{
	class PropertiesPanel
	{
	public:
		void SetScenePanel(ScenePanel* scenePanel);
		void Draw(bool* isOpen);

	private:
		ScenePanel* m_ScenePanel;
	};
}