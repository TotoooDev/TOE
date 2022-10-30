#include "pch.h"
#include <TOE/Editor/Panels/ViewportPanel.h>

namespace TOE
{
	void ViewportPanel::Init(ImVec2* viewportSize, Ref<Scene> scene, Ref<Framebuffer> framebuffer, Ref<EditorCamera> camera)
	{
		m_ViewportSize = viewportSize;
		m_CurrentScene = scene;
		m_Source = framebuffer;
		m_Camera = camera;
	}

	void ViewportPanel::Draw(bool* isOpen)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0.0f, 0.0f });
		ImGui::Begin(m_CurrentScene->Name.empty() ? "Scene" : m_CurrentScene->Name.c_str(), isOpen);
		m_Camera->SetViewportFocus(ImGui::IsWindowHovered());
		*m_ViewportSize = ImGui::GetContentRegionAvail();
		ImGui::Image((void*)m_Source->RetrieveTexture(0), ImGui::GetContentRegionAvail(), ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		ImGui::End();
		ImGui::PopStyleVar();
	}
}