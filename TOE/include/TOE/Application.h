#pragma once

#include "Window.h"
#include "Event/EventBus.h"
#include "Event/WindowEvents.h"

namespace TOE
{
	class Application
	{
	public:
		Application(const WindowData& data);
		~Application();

		static Application& Get();

		void Run();
		void Stop();

		EventBus EventBus;

	private:
		void OnWindowClosedEvent(WindowClosedEvent* event);
		
		static Application* m_Instance;

		Window m_Window;

		bool m_IsRunning = true;
	};
}