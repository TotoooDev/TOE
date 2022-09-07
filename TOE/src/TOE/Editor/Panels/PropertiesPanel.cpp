#include <TOE/Editor/Panels/PropertiesPanel.h>
#include <TOE/Scene/Components.h>
#include <ImGui/imgui.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>

namespace TOE
{
	void PropertiesPanel::SetScenePanel(ScenePanel* scenePanel)
	{
		m_ScenePanel = scenePanel;
	}

	void PropertiesPanel::Draw(bool* isOpen)
	{
		ImGui::Begin("Properties", isOpen);
		if (m_ScenePanel->m_SelectedEntity.IsValid())
		{
			Entity ent = m_ScenePanel->m_SelectedEntity;
			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed;

			if (ent.HasComponent<TagComponent>())
			{
				auto& tagComponent = ent.GetComponent<TagComponent>();
				if (ImGui::TreeNodeEx("Tag", flags))
				{
					char input[256];
					memset(input, 0, sizeof(input));
					memcpy(input, tagComponent.Tag.c_str(), tagComponent.Tag.size());
					if (ImGui::InputText(" ", input, sizeof(input)))
						tagComponent.Tag = input;
					ImGui::TreePop();
				}
			}

			if (ent.HasComponent<TransformComponent>())
			{
				auto& transformComponent = ent.GetComponent<TransformComponent>();
				if (ImGui::TreeNodeEx("Transform", flags))
				{
					ImGui::Text("Doesn't work lol");
					ImGui::DragFloat3("Pos", glm::value_ptr(transformComponent.Transform[3]), 0.1f);
					ImGui::TreePop();
				}
			}

			if (ent.HasComponent<RenderComponent>())
			{
				auto& renderComponent = ent.GetComponent<RenderComponent>();
				if (ImGui::TreeNodeEx("Render", flags))
				{
					ImGui::Image((void*)renderComponent.Texture->GetID(), ImVec2{ 256.0f, 256.0f }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
					ImGui::Checkbox("Render", &renderComponent.Render);
					ImGui::TreePop();
				}
			}

			if (ent.HasComponent<CameraComponent>())
			{
				auto& cameraComponent = ent.GetComponent<CameraComponent>();
				if (ImGui::TreeNodeEx("Camera", flags))
				{
					ImGui::Checkbox("Primary", &cameraComponent.Primary);
					ImGui::Checkbox("Fixed Aspect Ratio", &cameraComponent.FixedAspectRatio);
					ImGui::Checkbox("Orbiting Camera", &cameraComponent.OrbitingCamera);
					ImGui::TreePop();
				}
			}
		}
		ImGui::End();
	}
}