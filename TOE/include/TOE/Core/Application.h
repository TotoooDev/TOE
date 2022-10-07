#pragma once

#include <TOE/Core/Window.h>
#include <TOE/Core/Layer.h>
#include <TOE/Event/EventBus.h>
#include <TOE/Event/WindowEvents.h>
#include <TOE/Editor/ImGuiLayer.h>

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

		void LockMouse(bool lock);

		WindowData GetWindowData();
		Window GetWindow();
		double GetTimestep();

		EventBus EventBus;

	private:
		void OnWindowClosedEvent(WindowClosedEvent* event);
		void OnWindowResizedEvent(WindowResizedEvent* event);
		
		static Application* m_Instance;

		Window m_Window;
		std::vector<Layer*> m_Layers;
		ImGuiLayer* m_ImGuiLayer;

		bool m_IsRunning = true;
		bool m_Minimized = false;
		double m_Timestep = 0.0f;
		double m_LastFrame = 0.0f;
	};
}