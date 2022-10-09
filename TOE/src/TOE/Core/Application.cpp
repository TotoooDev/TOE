#include "pch.h"
#include <TOE/Core/Application.h>
#include <TOE/Core/Core.h>
#include <TOE/Core/Window.h>
#include <TOE/Core/GlobalConfig.h>
#include <TOE/Event/WindowEvents.h>
#include <TOE/Graphics/Renderer.h>

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

		// Initialize the config
		GlobalConfig::Init();

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

		// Sub to the window events
		EventBus.Subscribe(this, &Application::OnWindowClosedEvent);
		EventBus.Subscribe(this, &Application::OnWindowResizedEvent);

		m_ImGuiLayer = new ImGuiLayer;
		m_ImGuiLayer->OnCreate();
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
			m_ImGuiLayer->Begin();
			for (auto& layer : m_Layers)
			{
				layer->OnImGuiRender();
			}
			m_ImGuiLayer->End();

			m_Window.Update();

			// Update timestep
			double currentFrame = glfwGetTime();
			m_Timestep = currentFrame - m_LastFrame;
			m_LastFrame = currentFrame;
		}

		GlobalConfig::Write();
	}

	void Application::Stop()
	{
		TOE_PROFILE_FUNCTION();

		m_IsRunning = false;
	}

	void Application::LockMouse(bool lock)
	{
		m_Window.LockMouse(lock);
	}

	WindowData Application::GetWindowData()
	{
		return m_Window.GetData();
	}

	Window Application::GetWindow()
	{
		return m_Window;
	}

	double Application::GetTimestep()
	{
		return m_Timestep;
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