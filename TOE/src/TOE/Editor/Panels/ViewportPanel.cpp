#include "pch.h"
#include <TOE/Editor/Panels/ViewportPanel.h>

namespace TOE
{
	void ViewportPanel::Init(Ref<Scene> scene, Ref<Framebuffer> framebuffer, Ref<EditorCamera> camera)
	{
		m_CurrentScene = scene;
		m_Source = framebuffer;
		m_Camera = camera;
	}

	void ViewportPanel::Draw(bool* isOpen)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0.0f, 0.0f });
		ImGui::Begin(m_CurrentScene->Name.c_str(), isOpen);
		m_Camera->SetViewportFocus(ImGui::IsWindowHovered());
		ImVec2 currentViewportSize = ImGui::GetContentRegionAvail();
		if (currentViewportSize.x != m_ViewportSize.x || currentViewportSize.y != m_ViewportSize.y)
		{
			m_ViewportSize = currentViewportSize;
			m_CurrentScene->OnViewportResize((unsigned int)m_ViewportSize.x, (unsigned int)m_ViewportSize.y);
			m_Camera->OnViewportResize((unsigned int)m_ViewportSize.x, (unsigned int)m_ViewportSize.y);
		}
		ImGui::Image((void*)m_Source->GetColorAttachmentID(), ImGui::GetContentRegionAvail(), ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		ImGui::End();
		ImGui::PopStyleVar();
	}
}