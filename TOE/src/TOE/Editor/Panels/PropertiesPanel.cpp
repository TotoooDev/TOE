#include "pch.h"
#include <TOE/Editor/Panels/PropertiesPanel.h>
#include <TOE/Scene/Components.h>
#include <TOE/Graphics/Primitives.h>
#include <TOE/Utils/WindowsUtils.h>

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
					ImGui::DragFloat3("Position", glm::value_ptr(transformComponent.Translation), 0.1f);
					ImGui::DragFloat3("Rotation", glm::value_ptr(transformComponent.Rotation), 0.1f);
					ImGui::DragFloat3("Scale", glm::value_ptr(transformComponent.Scale), 0.1f);
					ImGui::TreePop();
				}
			}

			if (ent.HasComponent<MaterialComponent>())
			{
				auto& materialComponent = ent.GetComponent<MaterialComponent>();
				if (ImGui::TreeNodeEx("Render", flags))
				{
					DrawRemove<MaterialComponent>();

					for (auto& material : materialComponent.Materials)
					{
						ImGui::ColorEdit3("Color", glm::value_ptr(material.DiffuseColor));
						ImGui::Separator();
						ImGui::Text("Diffuse texture");
						if (material.Diffuse)
						{
							ImGui::Image((void*)material.Diffuse->GetID(), ImVec2{ 128, 128 }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
							if (ImGui::Button("Delete texture"))
							{
								material.Diffuse = { };
							}
						}
						if (ImGui::Button("Browse..."))
						{
							auto path = Utils::OpenFileDialog("Image files\0*.png;*.jpg;*.jpeg;*.bmp;*.gif");
							if (!material.Diffuse)
							{
								material.Diffuse = CreateRef<Texture2D>();
							}
							material.Diffuse->CreateFromFile(path);
						}
					}
					
					ImGui::Separator();
					ImGui::Checkbox("Use color", &materialComponent.UseColor);
					ImGui::TreePop();
				}
			}

			if (ent.HasComponent<MeshComponent>())
			{
				auto& meshComponent = ent.GetComponent<MeshComponent>();
				if (ImGui::TreeNodeEx("Mesh", flags))
				{
					DrawRemove<MeshComponent>();
					// if (ImGui::Button("Browse..."))
					// {
					// 	auto path = Utils::OpenFileDialog("Model files\0*.obj;*.fbx;*.gltf");
					// 	if (!meshComponent.Model)
					// 	{
					// 		meshComponent = CreateRef<Model>();
					// 	}
					// 	// material.Diffuse->CreateFromFile(path);
					// }
					// ImGui::Checkbox("Render", &meshComponent.Render);
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
					// if (ImGui::MenuItem("Material") && !m_ScenePanel->m_SelectedEntity.HasComponent<MaterialComponent>())
					// 	m_ScenePanel->m_SelectedEntity.AddComponent<MaterialComponent>();
					// if (ImGui::BeginMenu("Mesh"))
					// {
					// 	if (ImGui::MenuItem("Quad") && !m_ScenePanel->m_SelectedEntity.HasComponent<MeshComponent>())
					// 		m_ScenePanel->m_SelectedEntity.AddComponent<MeshComponent>(Primitives::GetQuadVAO(), Primitives::GetQuadEBO(), PrimitiveType::Quad);
					// 	if (ImGui::MenuItem("Cube") && !m_ScenePanel->m_SelectedEntity.HasComponent<MeshComponent>())
					// 		m_ScenePanel->m_SelectedEntity.AddComponent<MeshComponent>(Primitives::GetCubeVAO(), Primitives::GetCubeEBO(), PrimitiveType::Cube);
					// 	ImGui::EndMenu();
					// }
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