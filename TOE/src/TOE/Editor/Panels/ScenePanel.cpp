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

		if (ImGui::TreeNodeEx(m_CurrentScene->Name.empty() ? "Scene" : m_CurrentScene->Name.c_str(), ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick))
		{
			ImGuiPopupFlags flags = ImGuiPopupFlags_MouseButtonRight;
			if (ImGui::BeginPopupContextItem("Create Entity", flags))
			{
				if (ImGui::MenuItem("Create New Entity"))
					m_SelectedEntity = m_CurrentScene->CreateEntity();
				if (ImGui::MenuItem("Rename scene"))
					m_RenameScene = true;
				ImGui::EndPopup();
			}

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
					if (ImGui::BeginPopupContextItem())
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

			ImGui::TreePop();
		}

		if (m_RenameScene)
			ImGui::OpenPopup("Rename scene");
		// Always center this window when appearing
		ImVec2 center = ImGui::GetMainViewport()->GetCenter();
		ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
		if (ImGui::BeginPopupModal("Rename scene"))
		{
			std::string previousName = m_CurrentScene->Name;

			ImGui::Text("New name");
			char name[256];
			strcpy_s(name, m_CurrentScene->Name.c_str());
			if (ImGui::InputText("##", name, sizeof(name)))
				m_CurrentScene->Name = name;
			if (ImGui::Button("OK"))
			{
				m_RenameScene = false;
				ImGui::CloseCurrentPopup();
			}
			if (ImGui::Button("Cancel"))
			{
				m_CurrentScene->Name = name;
				m_RenameScene = false;
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}

		ImGui::End();
	}
}

