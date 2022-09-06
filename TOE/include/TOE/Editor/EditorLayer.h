#pragma once

#include <TOE/Core/Core.h>
#include <TOE/Core/Application.h>
#include <TOE/Core/EntryPoint.h>
#include <TOE/Core/Ref.h>
#include <TOE/Core/GlobalConfig.h>

#include <TOE/Editor/EditorLayer.h>
#include <TOE/Editor/EditorLog.h>
#include <TOE/Editor/EditorSink.h>

#include <TOE/Event/Events.h>

#include <TOE/Graphics/PerspectiveCamera.h>
#include <TOE/Graphics/Shader.h>
#include <TOE/Graphics/Texture.h>
#include <TOE/Graphics/Renderer.h>
#include <TOE/Graphics/Framebuffer.h>

#include <TOE/Scene/Scene.h>
#include <TOE/Scene/Entity.h>
#include <TOE/Scene/Components.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <ImGui/imgui.h>

namespace TOE
{
	class EditorLayer : public Layer
	{
	public:
		virtual void OnCreate() override
		{
			TOE_PROFILE_FUNCTION();

			// Add the editor sink to spdlog
			auto sink = std::make_shared<EditorSink>(&m_Console);
			sink->set_pattern("[%D %T] [%^%l%$] %v");
			spdlog::get("Default Logger")->sinks().push_back(sink);

			// Sub to the keyboard events
			Application::Get().EventBus.Subscribe(this, &EditorLayer::OnKeyPressed);
			Application::Get().EventBus.Subscribe(this, &EditorLayer::OnKeyUp);
			Application::Get().EventBus.Subscribe(this, &EditorLayer::OnWindowMaximized);
			Application::Get().EventBus.Subscribe(this, &EditorLayer::OnWindowRestored);

			// Load editor config
			m_ShowSettings = GlobalConfig::Get()["editor"]["show_settings"];
			m_ShowLogs = GlobalConfig::Get()["editor"]["show_logs"];

			Ref<Texture2D> texture = CreateRef<Texture2D>();
			Ref<VAO> vao = CreateRef<VAO>();
			Ref<EBO> ebo = CreateRef<EBO>();

			m_Framebuffer = CreateRef<Framebuffer>();

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

			m_Ent = m_Scene.CreateEntity();
			m_Ent.AddComponent<RenderComponent>(vao, ebo, texture);

			m_CamEnt = m_Scene.CreateEntity();
			auto& camComponent = m_CamEnt.AddComponent<CameraComponent>(CreateRef<PerspectiveCamera>(PerspectiveCamera()));
			camComponent.Primary = true;
			auto& transform = m_CamEnt.GetComponent<TransformComponent>();
			transform = glm::translate(transform.Transform, glm::vec3(0.0f, 0.0f, 2.0f));
		}

		virtual void OnUpdate(double timestep) override
		{
			auto& data = m_Framebuffer->GetFramebufferData();
			if ((data.Width != m_ViewportSize.x || data.Height != m_ViewportSize.y) &&
				(m_ViewportSize.x != 0 || m_ViewportSize.y != 0))
			{
				m_Framebuffer->Resize((unsigned int)m_ViewportSize.x, (unsigned int)m_ViewportSize.y);
			}

			m_Framebuffer->Use();
			m_Scene.Update(timestep);
			m_Framebuffer->Unbind();
		}

		virtual void OnImGuiRender() override
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
					if (ImGui::MenuItem("Settings", nullptr, &m_ShowSettings))
						GlobalConfig::Get()["editor"]["show_settings"] = m_ShowSettings;
					if (ImGui::MenuItem("Logs", nullptr, &m_ShowLogs))
						GlobalConfig::Get()["editor"]["show_logs"] = m_ShowLogs;
					ImGui::EndMenu();
				}
				ImGui::EndMainMenuBar();
			}

			// Viewport
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0.0f, 0.0f });
			ImGui::Begin("Scene viewport");
			ImVec2 currentViewportSize = ImGui::GetContentRegionAvail();
			if (currentViewportSize.x != m_ViewportSize.x || currentViewportSize.y != m_ViewportSize.y)
			{
				m_ViewportSize = currentViewportSize;
			}
			ImGui::Image((void*)m_Framebuffer->GetColorAttachmentID(), ImGui::GetContentRegionAvail(), ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
			ImGui::End();
			ImGui::PopStyleVar();

			// Settings window
			if (m_ShowSettings)
			{
				ImGui::Begin("Settings", &m_ShowSettings);
				auto stats = Renderer::GetStats();
				ImGui::Text("Renderer stats:");
				ImGui::Text("Draw calls: %d", stats.DrawCalls);
				ImGui::Text("Vertex count: %d", stats.VertexCount);
				ImGui::Text("Index count: %d", stats.IndexCount);
				ImGui::End();
			}

			// Log console
			if (m_ShowLogs)
				m_Console.Draw("Logs", &m_ShowLogs);
		}

	private:
		void OnKeyPressed(KeyDownEvent* event)
		{
			// switch (event->Keycode)
			// {
			// default:
			// 	break;
			// }
		}
		void OnKeyUp(KeyUpEvent* event)
		{
			// switch (event->Keycode)
			// {
			// default:
			// 	break;
			// }
		}
		void OnWindowMaximized(WindowMaximizedEvent* event)
		{
			GlobalConfig::Get()["window"]["start_maximized"] = true;
		}
		void OnWindowRestored(WindowRestoredEvent* event)
		{
			GlobalConfig::Get()["window"]["start_maximized"] = false;
		}

		Ref<Framebuffer> m_Framebuffer;
		ImVec2 m_ViewportSize;

		Scene m_Scene;
		Entity m_Ent, m_CamEnt;

		bool m_ShowLogs;
		bool m_ShowSettings;
		EditorLogConsole m_Console;
	};
}