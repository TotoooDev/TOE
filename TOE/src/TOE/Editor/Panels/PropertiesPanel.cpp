#include "pch.h"
#include <TOE/Editor/Panels/PropertiesPanel.h>
#include <TOE/Scene/Components.h>
#include <TOE/Scene/Importer.h>
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
					if (ImGui::InputText("##", input, sizeof(input)))
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
					ImGui::DragFloat3("Scale", glm::value_ptr(transformComponent.Scale), 0.01f);
					ImGui::TreePop();
				}
			}

			if (ent.HasComponent<MaterialComponent>())
			{
				auto& materialComponent = ent.GetComponent<MaterialComponent>();
				if (ImGui::TreeNodeEx("Textures", flags))
				{
					DrawRemove<MaterialComponent>();

					for (auto& material : materialComponent.Materials)
					{
						if (ImGui::TreeNode(material.Name.c_str()))
						{
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
								if (!path.empty())
									material.Diffuse->CreateFromFile(path);
							}
							ImGui::TreePop();
						}
					}
					ImGui::TreePop();
				}
			}

			if (ent.HasComponent<MeshComponent>())
			{
				auto& meshComponent = ent.GetComponent<MeshComponent>();
				if (ImGui::TreeNodeEx("Mesh", flags))
				{
					DrawRemove<MeshComponent>();
					ImGui::Text("Path: %s", meshComponent.Model->GetPath().c_str());
					ImGui::Separator();
					if (ImGui::Button("Browse..."))
					{
						auto path = Utils::OpenFileDialog("Model files\0*.obj;*.fbx;*.gltf");
						if (!path.empty())
						{
							Importer importer(path);
							auto&& [model, materials] = importer.LoadModelAndMaterialFromFile();
							meshComponent.Model = model;
							if (!ent.HasComponent<MaterialComponent>())
								ent.AddComponent<MaterialComponent>(materials);
							else
								ent.GetComponent<MaterialComponent>().Materials = materials;
						}
					}
					ImGui::Checkbox("Render", &meshComponent.Render);
					ImGui::TreePop();
				}
			}

			if (ent.HasComponent<LightComponent>())
			{
				auto& lightComponent = ent.GetComponent<LightComponent>();
				if (ImGui::TreeNodeEx("Light", flags))
				{
					DrawRemove<LightComponent>();
					const char* types[] = {"Directionnal light", "Point light", "Spot light"};
					ImGui::Combo("Light type", &lightComponent.Light.Type, types, 3);
					ImGui::Separator();
					ImGui::ColorEdit3("Ambient", glm::value_ptr(lightComponent.Light.Ambient));
					ImGui::ColorEdit3("Diffuse", glm::value_ptr(lightComponent.Light.Diffuse));
					ImGui::ColorEdit3("Specular", glm::value_ptr(lightComponent.Light.Specular));
					ImGui::Separator();
					ImGui::DragFloat("Constant", &lightComponent.Light.Constant, 0.001f, 0.0f, 0.0f, "%.5f");
					ImGui::DragFloat("Linear", &lightComponent.Light.Linear, 0.001f, 0.0f, 0.0f, "%.5f");
					ImGui::DragFloat("Quadratic", &lightComponent.Light.Quadratic, 0.001f, 0.0f, 0.0f, "%.5f");
					ImGui::Separator();
					ImGui::DragFloat("Cut off", &lightComponent.Light.CutOff, 0.01f);
					ImGui::DragFloat("Outer cut off", &lightComponent.Light.OuterCutOff, 0.01f);
					ImGui::Separator();
					ImGui::Checkbox("Emit", &lightComponent.Emit);
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
					if (ImGui::MenuItem("Material") && !m_ScenePanel->m_SelectedEntity.HasComponent<MaterialComponent>())
					{
						Material mat;
						std::vector<Material> materials =
						{
							mat
						};
						m_ScenePanel->m_SelectedEntity.AddComponent<MaterialComponent>(materials);
					}
					if (ImGui::BeginMenu("Mesh"))
					{
						if (ImGui::MenuItem("Plane") && !m_ScenePanel->m_SelectedEntity.HasComponent<MeshComponent>())
						{
							Importer importer("assets/plane.fbx");
							m_ScenePanel->m_SelectedEntity.AddComponent<MeshComponent>(importer.LoadModelFromFile());
						}
						if (ImGui::MenuItem("Cube") && !m_ScenePanel->m_SelectedEntity.HasComponent<MeshComponent>())
						{
							Importer importer("assets/cube.fbx");
							m_ScenePanel->m_SelectedEntity.AddComponent<MeshComponent>(importer.LoadModelFromFile());
						}
						if (ImGui::MenuItem("Browse...") && !m_ScenePanel->m_SelectedEntity.HasComponent<MeshComponent>())
						{
							auto path = Utils::OpenFileDialog("Model files\0*.obj;*.fbx;*.gltf");
							auto& meshComponent = ent.AddComponent<MeshComponent>();
							if (!path.empty())
							{
								Importer importer(path);
								auto&& [model, materials] = importer.LoadModelAndMaterialFromFile();
								meshComponent.Model = model;
								if (!ent.HasComponent<MaterialComponent>())
									ent.AddComponent<MaterialComponent>(materials);
								if (ent.HasComponent<MaterialComponent>() && materials.size() == 0)
									ent.RemoveComponent<MaterialComponent>();
							}
						}
						ImGui::EndMenu();
					}
					if (ImGui::MenuItem("Light") && !m_ScenePanel->m_SelectedEntity.HasComponent<LightComponent>())
						m_ScenePanel->m_SelectedEntity.AddComponent<LightComponent>();
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