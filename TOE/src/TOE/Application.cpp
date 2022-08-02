#include <TOE/Application.h>
#include <TOE/Window.h>
#include <TOE/Event/WindowEvents.h>

#include <spdlog/spdlog.h>

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

		m_Window.CreateNewWindow(data);
		m_Window.SetEventBus(&EventBus);

		EventBus.Subscribe(this, &Application::OnWindowClosedEvent);
	}

	Application::~Application()
	{

	}

	Application& Application::Get()
	{
		return *m_Instance;
	}

	void Application::Run()
	{
		while (m_IsRunning)
		{
			m_Window.Update();
		}
	}

	void Application::Stop()
	{
		m_IsRunning = false;
	}

	void Application::OnWindowClosedEvent(WindowClosedEvent* event)
	{
		m_IsRunning = false;
	}
}