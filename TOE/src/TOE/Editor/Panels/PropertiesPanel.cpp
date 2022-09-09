#include <TOE/Editor/Panels/PropertiesPanel.h>
#include <TOE/Scene/Components.h>
#include <TOE/Graphics/Primitives.h>
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
			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

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
					DrawRemove<TransformComponent>();
					ImGui::Text("Doesn't work lol");
					ImGui::DragFloat3("Position", glm::value_ptr(transformComponent.Translation), 0.1f);
					ImGui::DragFloat3("Rotation", glm::value_ptr(transformComponent.Rotation), 0.1f);
					ImGui::DragFloat3("Scale", glm::value_ptr(transformComponent.Scale), 0.1f);
					ImGui::TreePop();
				}
			}

			if (ent.HasComponent<RenderComponent>())
			{
				auto& renderComponent = ent.GetComponent<RenderComponent>();
				if (ImGui::TreeNodeEx("Render", flags))
				{
					DrawRemove<RenderComponent>();
					ImGui::Checkbox("Render", &renderComponent.Render);
					ImGui::ColorEdit3("Color", glm::value_ptr(renderComponent.Color));
					ImGui::TreePop();
				}
			}

			if (ent.HasComponent<CameraComponent>())
			{
				auto& cameraComponent = ent.GetComponent<CameraComponent>();
				if (ImGui::TreeNodeEx("Camera", flags))
				{
					DrawRemove<CameraComponent>();
					ImGui::Checkbox("Primary", &cameraComponent.Primary);
					ImGui::Checkbox("Fixed Aspect Ratio", &cameraComponent.FixedAspectRatio);
					ImGui::Checkbox("Orbiting Camera", &cameraComponent.OrbitingCamera);
					ImGui::TreePop();
				}
			}

			flags = ImGuiPopupFlags_NoOpenOverItems | ImGuiPopupFlags_MouseButtonRight;
			if (ImGui::BeginPopupContextWindow("Add Component", flags))
			{
				if (ImGui::BeginMenu("Add component"))
				{
					if (ImGui::MenuItem("Transform") && !m_ScenePanel->m_SelectedEntity.HasComponent<TransformComponent>())
						m_ScenePanel->m_SelectedEntity.AddComponent<TransformComponent>();
					if (ImGui::MenuItem("Render") && !m_ScenePanel->m_SelectedEntity.HasComponent<RenderComponent>())
						m_ScenePanel->m_SelectedEntity.AddComponent<RenderComponent>(Primitives::GetQuadVAO(), Primitives::GetQuadEBO());
					if (ImGui::MenuItem("Camera") && !m_ScenePanel->m_SelectedEntity.HasComponent<CameraComponent>())
						m_ScenePanel->m_SelectedEntity.AddComponent<CameraComponent>(CreateRef<PerspectiveCamera>());
					ImGui::EndMenu();
				}
				ImGui::EndPopup();
			}
		}
		ImGui::End();
	}
}