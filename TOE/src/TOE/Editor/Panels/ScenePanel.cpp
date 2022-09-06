#include <TOE/Editor/Panels/ScenePanel.h>
#include <TOE/Scene/Components.h>
#include <ImGui/imgui.h>

namespace TOE
{
	void ScenePanel::SetCurrentScene(Ref<Scene> scene)
	{
		m_CurrentScene = scene;
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
			
			if (ImGui::IsItemClicked())
			{
				m_SelectedEntity = ent;
			}

			if (opened)
			{
				ImGui::TreePop();
			}
		}

		ImGui::End();
	}
}

