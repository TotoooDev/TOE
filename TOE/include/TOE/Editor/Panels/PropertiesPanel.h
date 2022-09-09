#pragma once

#include <TOE/Core/Ref.h>
#include <TOE/Editor/Panels/ScenePanel.h>
#include <ImGui/imgui.h>

namespace TOE
{
	class PropertiesPanel
	{
	public:
		void SetScenePanel(ScenePanel* scenePanel);
		void Draw(bool* isOpen);

	private:
		template <typename T>
		void DrawRemove()
		{
			if (ImGui::BeginPopupContextWindow())
			{
				if (ImGui::MenuItem("Remove Component"))
				{
					m_ScenePanel->m_SelectedEntity.RemoveComponent<T>();
				}
				ImGui::EndPopup();
			}
		}

		ScenePanel* m_ScenePanel;
	};
}