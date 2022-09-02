#include <TOE/Core/Application.h>
#include <TOE/Core/Core.h>
#include <TOE/Core/Window.h>
#include <TOE/Event/WindowEvents.h>
#include <TOE/Graphics/Renderer.h>
#include <TOE/Debug/Instrumentor.h>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_glfw.h>
#include <ImGui/imgui_impl_opengl3.h>

namespace TOE
{
	Application* Application::m_Instance = nullptr;

	Application::Application(const WindowData& data)
	{
		TOE_PROFILE_FUNCTION();

		if (m_Instance == nullptr)
		{
			m_Instance = this;
		}
		else
		{
			spdlog::error("An application already exists!");
		}

		// spdlog setup
		auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
		consoleSink->set_pattern("[%D %T] [%^%l%$] %v");
		auto fileSink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>("logs/TOE_Logs.log",1048576 * 5, 3);
		fileSink->set_pattern("[%D %T] [%^%l%$] %v");
		spdlog::logger logger("Default Logger", { consoleSink, fileSink });
		spdlog::set_default_logger(std::make_shared<spdlog::logger>(logger));

		// Create a window
		m_Window.CreateNewWindow(data);
		m_Window.SetEventBus(&EventBus);

		// Initialize renderer
		Renderer::Init();

		// Sub to the window events
		EventBus.Subscribe(this, &Application::OnWindowClosedEvent);
		EventBus.Subscribe(this, &Application::OnWindowResizedEvent);
	}

	Application::~Application()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	Application& Application::Get()
	{
		return *m_Instance;
	}

	void Application::AddLayer(Layer* layer)
	{
		TOE_PROFILE_FUNCTION();

		m_Layers.push_back(layer);
		layer->OnCreate();
	}

	void Application::Run()
	{
		TOE_PROFILE_FUNCTION();

		while (m_IsRunning)
		{
			if (!m_Minimized)
			{
				// Update all layers
				for (auto& layer : m_Layers)
				{
					layer->OnUpdate(m_Timestep);
				}
			}

			// ImGui
			ImGuiBegin();
			for (auto& layer : m_Layers)
			{
				layer->OnImGuiRender();
			}
			ImGuiEnd();

			m_Window.Update();

			// Update timestep
			double currentFrame = glfwGetTime();
			m_Timestep = currentFrame - m_LastFrame;
			m_LastFrame = currentFrame;
		}
	}

	void Application::Stop()
	{
		TOE_PROFILE_FUNCTION();

		m_IsRunning = false;
	}

	WindowData Application::GetWindowData()
	{
		return m_Window.GetData();
	}

	void Application::ImGuiBegin()
	{
		TOE_PROFILE_FUNCTION();

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void Application::ImGuiEnd()
	{
		TOE_PROFILE_FUNCTION();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		ImGuiIO& io = ImGui::GetIO(); (void)io;
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}

	void Application::OnWindowClosedEvent(WindowClosedEvent* event)
	{
		Stop();
	}
	void Application::OnWindowResizedEvent(WindowResizedEvent* event)
	{
		if (event->Width == 0 && event->Height == 0)
			m_Minimized = true;
		else
			m_Minimized = false;
		glViewport(0, 0, m_Window.GetData().Width, m_Window.GetData().Height);
	}
}