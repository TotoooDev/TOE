#include "pch.h"
#include <TOE/Editor/EditorLayer.h>
#include <TOE/Core/Application.h>
#include <TOE/Core/GlobalConfig.h>
#include <TOE/Editor/EditorSink.h>
#include <TOE/Event/Input.h>
#include <TOE/Graphics/Renderer.h>
#include <TOE/Scene/Components.h>
#include <TOE/Scene/Serializer.h>
#include <TOE/Utils/WindowsUtils.h>

#include <TOE/Scene/Importer.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace TOE
{
	void EditorLayer::OnCreate()
	{
		TOE_PROFILE_FUNCTION();

		// Add the editor sink to spdlog
		auto sink = std::make_shared<EditorSink>(&m_ConsolePanel);
		sink->set_pattern("[%D %T] [%^%l%$] %v");
		spdlog::get("Default Logger")->sinks().push_back(sink);

		// Sub to the keyboard events
		Application::Get().EventBus.Subscribe(this, &EditorLayer::OnKeyPressed);
		Application::Get().EventBus.Subscribe(this, &EditorLayer::OnKeyUp);
		Application::Get().EventBus.Subscribe(this, &EditorLayer::OnWindowMaximized);
		Application::Get().EventBus.Subscribe(this, &EditorLayer::OnWindowRestored);

		// Load editor config
		m_ShowRenderStatsPanel = GlobalConfig::Get()["editor"]["show_renderer_stats"];
		m_ShowLogsPanel = GlobalConfig::Get()["editor"]["show_logs"];
		m_ShowScenePanel = GlobalConfig::Get()["editor"]["show_scene_hierarchy"];
		m_ShowViewportPanel = GlobalConfig::Get()["editor"]["show_viewport"];
		m_ShowPropertiesPanel = GlobalConfig::Get()["editor"]["show_properties"];

		m_Framebuffer = CreateRef<Framebuffer>();
		m_Scene = CreateRef<Scene>();
		m_Camera = CreateRef<EditorCamera>();
		m_Camera->Sensibility = GlobalConfig::Get()["editor"]["camera"]["sensibility"];

		m_ScenePanel.SetCurrentScene(m_Scene);
		m_ViewportPanel.Init(m_Scene, m_Framebuffer, m_Camera);
		m_PropertiesPanel.SetScenePanel(&m_ScenePanel);
		m_SettingsPanel.Init(m_Camera);

		FramebufferData data;
		data.Width = Application::Get().GetWindowData().Width;
		data.Height = Application::Get().GetWindowData().Height;
		m_Framebuffer->Create(data);
	}

	void EditorLayer::OnUpdate(double timestep)
	{
		m_Camera->OnUpdate(timestep);

		auto& data = m_Framebuffer->GetFramebufferData();
		if ((data.Width != m_ViewportSize.x || data.Height != m_ViewportSize.y) &&
			(m_ViewportSize.x != 0 || m_ViewportSize.y != 0))
		{
			m_Framebuffer->Resize((unsigned int)m_ViewportSize.x, (unsigned int)m_ViewportSize.y);
		}

		m_Framebuffer->Use();
		m_Scene->UpdateEditor(timestep, m_Camera);
		m_Framebuffer->Unbind();
	}

	void EditorLayer::OnImGuiRender()
	{
		TOE_PROFILE_FUNCTION();

		ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
		// Menu bar
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New Scene", "Ctrl+N"))
				{
					m_NewScene = true;
				}
				if (ImGui::MenuItem("Save", "Ctrl+S"))
				{
					Save();
				}
				if (ImGui::MenuItem("Save as...", "Ctrl+Shift+S"))
				{
					SaveAs();
				}
				if (ImGui::MenuItem("Open...", "Ctrl+O"))
				{
					m_OpenScene = true;
				}
				ImGui::Separator();
				if (ImGui::MenuItem("Close"))
					Application::Get().Stop();
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("View"))
			{
				if (ImGui::MenuItem("Renderer Stats", nullptr, &m_ShowRenderStatsPanel))
					GlobalConfig::Get()["editor"]["show_renderer_stats"] = m_ShowRenderStatsPanel;
				if (ImGui::MenuItem("Logs", nullptr, &m_ShowLogsPanel))
					GlobalConfig::Get()["editor"]["show_logs"] = m_ShowLogsPanel;
				if (ImGui::MenuItem("Scene Hierarchy", nullptr, &m_ShowRenderStatsPanel))
					GlobalConfig::Get()["editor"]["show_scene_hierarchy"] = m_ShowScenePanel;
				if (ImGui::MenuItem("Viewport", nullptr, &m_ShowViewportPanel))
					GlobalConfig::Get()["editor"]["show_viewport"] = m_ShowViewportPanel;
				if (ImGui::MenuItem("Properties", nullptr, &m_ShowPropertiesPanel))
					GlobalConfig::Get()["editor"]["show_properties"] = m_ShowPropertiesPanel;
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Preferences"))
			{
				if (ImGui::MenuItem("Settings"))
				{
					m_ShowSettingsPanel = true;
				}
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}

		if (m_NewScene || m_OpenScene)
			ImGui::OpenPopup("Warning");
		// Always center this window when appearing
		ImVec2 center = ImGui::GetMainViewport()->GetCenter();
		ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

		if (ImGui::BeginPopupModal("Warning", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("All unsaved data will be deleted!\nThis operation cannot be undone!\n\n");
			ImGui::Separator();

			if (ImGui::Button("OK", ImVec2(120, 0)))
			{
				if (m_NewScene)
				{
					New();
					m_NewScene = false;
				}
				if (m_OpenScene)
				{
					Open();
					m_OpenScene = false;
				}
				ImGui::CloseCurrentPopup();
			}
			ImGui::SetItemDefaultFocus();
			ImGui::SameLine();
			if (ImGui::Button("Cancel", ImVec2(120, 0)))
			{
				ImGui::CloseCurrentPopup();
				m_NewScene = false;
				m_OpenScene = false;
			}
			ImGui::EndPopup();
		}

		// Viewport
		if (m_ShowViewportPanel)
			m_ViewportPanel.Draw(&m_ShowViewportPanel);

		// Settings window
		if (m_ShowRenderStatsPanel)
			m_StatsPanel.Draw(&m_ShowRenderStatsPanel);

		// Log console
		if (m_ShowLogsPanel)
			m_ConsolePanel.Draw(&m_ShowLogsPanel);

		// Scene panel
		if (m_ShowScenePanel)
			m_ScenePanel.Draw(&m_ShowScenePanel);

		// Properties panel
		if (m_ShowPropertiesPanel)
			m_PropertiesPanel.Draw(&m_ShowPropertiesPanel);

		// Settings panel
		if (m_ShowSettingsPanel)
			m_SettingsPanel.Draw(&m_ShowSettingsPanel);
	}

	void EditorLayer::New()
	{
		m_Scene = CreateRef<Scene>();
		m_ScenePanel.SetCurrentScene(m_Scene);
		m_ViewportPanel.Init(m_Scene, m_Framebuffer, m_Camera);
		m_PropertiesPanel.SetScenePanel(&m_ScenePanel);
		m_ScenePath = "";
	}
	void EditorLayer::Save()
	{
		if (m_ScenePath.empty())
		{
			SaveAs();
		}
		else
		{
			SceneSerializer serializer(m_Scene);
			serializer.Serialize(m_ScenePath);
		}
	}
	void EditorLayer::SaveAs()
	{
		m_ScenePath = Utils::SaveFileDialog("TOE Scene file (*.toe)\0*.toe\0");
		SceneSerializer serializer(m_Scene);
		serializer.Serialize(m_ScenePath);
	}
	void EditorLayer::Open()
	{
		auto path = Utils::OpenFileDialog("TOE Scene file (*.toe)\0*.toe\0");
		if (!path.empty())
		{
			m_ScenePath = path;
			m_Scene = CreateRef<Scene>();
			SceneSerializer serializer(m_Scene);
			serializer.Deserialize(m_ScenePath);
			m_ScenePanel.SetCurrentScene(m_Scene);
			m_ViewportPanel.Init(m_Scene, m_Framebuffer, m_Camera);
			m_PropertiesPanel.SetScenePanel(&m_ScenePanel);
		}
	}

	void EditorLayer::OnKeyPressed(KeyDownEvent* event)
	{
		switch (event->Keycode)
		{
		case TOE_KEY_S:
			if (Input::Key(TOE_KEY_LEFT_CONTROL) || Input::Key(TOE_KEY_RIGHT_CONTROL))
			{
				if (Input::Key(TOE_KEY_LEFT_SHIFT) || Input::Key(TOE_KEY_RIGHT_SHIFT))
					SaveAs();
				else
					Save();
			}
			break;

		case TOE_KEY_O:
			if (Input::Key(TOE_KEY_LEFT_CONTROL) || Input::Key(TOE_KEY_RIGHT_CONTROL))
			{
				m_OpenScene = true;
			}
			break;
		case TOE_KEY_N:
			if (Input::Key(TOE_KEY_LEFT_CONTROL) || Input::Key(TOE_KEY_RIGHT_CONTROL))
			{
				m_NewScene = true;
			}
			break;

		default:
			break;
		}
	}
	void EditorLayer::OnKeyUp(KeyUpEvent* event)
	{
		// switch (event->Keycode)
		// {
		// default:
		// 	break;
		// }
	}
	void EditorLayer::OnWindowMaximized(WindowMaximizedEvent* event)
	{
		GlobalConfig::Get()["window"]["start_maximized"] = true;
	}
	void EditorLayer::OnWindowRestored(WindowRestoredEvent* event)
	{
		GlobalConfig::Get()["window"]["start_maximized"] = false;
	}
}