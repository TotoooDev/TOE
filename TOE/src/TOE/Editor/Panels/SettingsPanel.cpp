#include <TOE/Editor/Panels/SettingsPanel.h>
#include <TOE/Core/GlobalConfig.h>
#include <ImGui/imgui.h>

namespace TOE
{
	void SettingsPanel::Init(Ref<EditorCamera> cam)
	{
		m_Camera = cam;
	}

	// TODO: improve the looks
	void SettingsPanel::Draw(bool* isOpen)
	{
		ImGui::Begin("Settings", isOpen);
		if (ImGui::DragFloat("Camera sensibility", &m_Camera->Sensibility, 0.1f, 0.0f, 100.0f))
			GlobalConfig::Get()["editor"]["camera"]["sensibility"] = m_Camera->Sensibility;
		ImGui::End();
	}
}