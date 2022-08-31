#include <TOE/Core/Application.h>
#include <TOE/Core/Core.h>
#include <TOE/Core/Window.h>
#include <TOE/Event/WindowEvents.h>
#include <TOE/Graphics/Renderer.h>

#include <spdlog/spdlog.h>
#include "spdlog/sinks/rotating_file_sink.h"

namespace TOE
{
	Application* Application::m_Instance = nullptr;

	Application::Application(const WindowData& data)
	{
		if (m_Instance == nullptr)
		{
			m_Instance = this;
		}
		else
		{
			spdlog::error("An application already exists!");
		}
		// Create a file rotating logger with 5mb size max and 3 rotated files.
		auto rotating_logger = spdlog::rotating_logger_mt("Default Logger", "logs/TOE_logs.txt", 1048576 * 5, 3);
		spdlog::set_default_logger(rotating_logger);

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

	}

	Application& Application::Get()
	{
		return *m_Instance;
	}

	void Application::AddLayer(Layer* layer)
	{
		m_Layers.push_back(layer);
		layer->OnCreate();
	}

	void Application::Run()
	{
		while (m_IsRunning)
		{
			m_Window.Update();

			// Update all layers
			for (auto& layer : m_Layers)
			{
				layer->OnUpdate(m_Timestep);
			}
			// Check for OpenGL errors
			glCheckError();

			// Update timestep
			double currentFrame = glfwGetTime();
			m_Timestep = currentFrame - m_LastFrame;
			m_LastFrame = currentFrame;
		}
	}

	void Application::Stop()
	{
		m_IsRunning = false;
	}

	void Application::OnWindowClosedEvent(WindowClosedEvent* event)
	{
		Stop();
	}
	void Application::OnWindowResizedEvent(WindowResizedEvent* event)
	{
		glViewport(0, 0, m_Window.GetData().Width, m_Window.GetData().Height);
	}
}