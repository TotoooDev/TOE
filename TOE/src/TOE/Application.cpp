#include <TOE/Application.h>

#include <spdlog/spdlog.h>

namespace TOE
{
	Application* Application::m_Instance = nullptr;

	Application::Application()
	{
		if (m_Instance == nullptr)
		{
			m_Instance = this;
		}
		else
		{
			spdlog::error("An application already exists!");
		}
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
		spdlog::info("Application is running");
	}

	void Application::Stop()
	{

	}
}