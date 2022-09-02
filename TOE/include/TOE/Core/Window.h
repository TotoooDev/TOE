#pragma once

#include <TOE/Event/EventBus.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>

namespace TOE
{
	static bool WasGLFWInit = false;
	static bool WasGLEWInit = false;

	// Simple struct to hold all the useful window data
	struct WindowData
	{
		unsigned int Width;
		unsigned int Height;
		std::string Title;
		EventBus* EventBus = nullptr;

		WindowData()
			: Width(1280), Height(720), Title("TOE Application")
		{

		}
	};

	class Window
	{
	public:
		void CreateNewWindow(const WindowData& data);
		void Update();
		
		WindowData GetData();
		void SetData(const WindowData& data);
		void SetEventBus(EventBus* bus);

	private:
		void InitOpenGLDebugOutput();
		void InitImGui();
		void SetCallbacks();

		GLFWwindow* m_NativeWindow = nullptr;
		WindowData m_Data;
	};
}