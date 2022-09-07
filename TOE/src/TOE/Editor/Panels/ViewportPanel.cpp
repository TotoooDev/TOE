#include <TOE/Editor/Panels/ViewportPanel.h>

namespace TOE
{
	void ViewportPanel::Init(Ref<Scene> scene, Ref<Framebuffer> framebuffer, bool* hovered)
	{
		m_CurrentScene = scene;
		m_Source = framebuffer;
		m_Hovered = hovered;
	}

	void ViewportPanel::Draw(bool* isOpen)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0.0f, 0.0f });
		ImGui::Begin("Scene viewport", isOpen);
		*m_Hovered = ImGui::IsWindowHovered();
		ImVec2 currentViewportSize = ImGui::GetContentRegionAvail();
		if (currentViewportSize.x != m_ViewportSize.x || currentViewportSize.y != m_ViewportSize.y)
		{
			m_ViewportSize = currentViewportSize;
			m_CurrentScene->OnViewportResize((unsigned int)m_ViewportSize.x, (unsigned int)m_ViewportSize.y);
		}
		ImGui::Image((void*)m_Source->GetColorAttachmentID(), ImGui::GetContentRegionAvail(), ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		ImGui::End();
		ImGui::PopStyleVar();
	}
}