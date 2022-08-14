#pragma once

#include <TOE/Core/Window.h>
#include <TOE/Core/Layer.h>
#include <TOE/Event/EventBus.h>
#include <TOE/Event/WindowEvents.h>

#include <vector>

namespace TOE
{
	class Application
	{
	public:
		Application(const WindowData& data);
		~Application();

		// Return the instance of the application
		static Application& Get();
		void AddLayer(Layer* layer);

		// Run the main loop of the application
		void Run();
		// Close and clean the application
		void Stop();

		EventBus EventBus;

	private:
		void OnWindowClosedEvent(WindowClosedEvent* event);
		void OnWindowResizedEvent(WindowResizedEvent* event);
		
		static Application* m_Instance;

		Window m_Window;
		std::vector<Layer*> m_Layers;

		bool m_IsRunning = true;
		double m_Timestep = 0.0f;
		double m_LastFrame = 0.0f;
	};
}