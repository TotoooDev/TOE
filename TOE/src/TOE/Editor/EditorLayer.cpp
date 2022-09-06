#include <TOE/Editor/EditorLayer.h>
#include <TOE/Core/Application.h>
#include <TOE/Core/GlobalConfig.h>
#include <TOE/Editor/EditorSink.h>
#include <TOE/Debug/Instrumentor.h>
#include <TOE/Graphics/Renderer.h>
#include <TOE/Scene/Components.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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

		Ref<Texture2D> texture = CreateRef<Texture2D>();
		Ref<VAO> vao = CreateRef<VAO>();
		Ref<EBO> ebo = CreateRef<EBO>();

		m_Framebuffer = CreateRef<Framebuffer>();
		m_Scene = CreateRef<Scene>();
		m_ScenePanel.SetCurrentScene(m_Scene);
		m_ViewportPanel.Init(m_Scene, m_Framebuffer);

		texture->CreateFromFile("textures/image.png");

		FramebufferData data;
		data.Width = Application::Get().GetWindowData().Width;
		data.Height = Application::Get().GetWindowData().Height;
		m_Framebuffer->Create(data);

		// OpenGL data setup
		std::vector<float> vertices =
		{
			// positions          // colors           // texture coords
			 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
			 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
			-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
			-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
		};
		std::vector<unsigned int> indices =
		{
			0, 1, 3, // first triangle
			1, 2, 3  // second triangle
		};

		VertexLayout layout;
		layout.AddAttribute(Type::Float, 3); // Pos
		layout.AddAttribute(Type::Float, 3); // Color
		layout.AddAttribute(Type::Float, 2); // Tex Coords

		vao->SetData(vertices, layout);
		ebo->SetData(indices);

		m_Ent = m_Scene->CreateEntity("Toto Entity");
		m_Ent.AddComponent<RenderComponent>(vao, ebo, texture);

		m_CamEnt = m_Scene->CreateEntity("Camera");
		auto& camComponent = m_CamEnt.AddComponent<CameraComponent>(CreateRef<PerspectiveCamera>(PerspectiveCamera()));
		camComponent.Primary = true;
		auto& transform = m_CamEnt.GetComponent<TransformComponent>();
		transform = glm::translate(transform.Transform, glm::vec3(0.0f, 0.0f, 2.0f));
	}

	void EditorLayer::OnUpdate(double timestep)
	{
		auto& data = m_Framebuffer->GetFramebufferData();
		if ((data.Width != m_ViewportSize.x || data.Height != m_ViewportSize.y) &&
			(m_ViewportSize.x != 0 || m_ViewportSize.y != 0))
		{
			m_Framebuffer->Resize((unsigned int)m_ViewportSize.x, (unsigned int)m_ViewportSize.y);
		}

		m_Framebuffer->Use();
		m_Scene->Update(timestep);
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
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
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
	}

	void EditorLayer::OnKeyPressed(KeyDownEvent* event)
	{
		// switch (event->Keycode)
		// {
		// default:
		// 	break;
		// }
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