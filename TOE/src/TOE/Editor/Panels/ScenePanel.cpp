#include "pch.h"
#include <TOE/Editor/Panels/ScenePanel.h>
#include <TOE/Scene/Components.h>
#include <ImGui/imgui.h>

namespace TOE
{
	void ScenePanel::SetCurrentScene(Ref<Scene> scene)
	{
		m_CurrentScene = scene;
		m_SelectedEntity = { };
	}
	
	void ScenePanel::Draw(bool* isOpen)
	{
		ImGui::Begin("Scene Hierarchy", isOpen);

		auto view = m_CurrentScene->m_Registry.view<TagComponent>();
		for (auto&& [entity, tagComponent] : view.each())
		{
			Entity ent(entity, m_CurrentScene.get());

			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
			if (m_SelectedEntity == ent)
			{
				flags |= ImGuiTreeNodeFlags_Selected;
			}
			bool opened = ImGui::TreeNodeEx((void*)(uint64_t)entity, flags, tagComponent.Tag.c_str());
			
			if (ImGui::IsItemClicked() || ImGui::IsItemClicked(ImGuiMouseButton_Right))
			{
				m_SelectedEntity = ent;
			}
			if (m_SelectedEntity == ent)
			{
				if (ImGui::BeginPopupContextWindow())
				{
					if (ImGui::MenuItem("Remove Entity"))
					{
						m_CurrentScene->DeleteEntity(m_SelectedEntity);
						m_SelectedEntity = { };
					}
					ImGui::EndPopup();
				}
			}

			if (opened)
			{
				ImGui::TreePop();
			}
		}

		ImGuiPopupFlags flags = ImGuiPopupFlags_NoOpenOverItems | ImGuiPopupFlags_MouseButtonRight;
		if (ImGui::BeginPopupContextWindow("Create Entity", flags))
		{
			if (ImGui::MenuItem("Create New Entity"))
				m_SelectedEntity = m_CurrentScene->CreateEntity();
			ImGui::EndPopup();
		}
		
		ImGui::End();
	}
}

