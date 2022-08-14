#include <TOE/Core/Window.h>
#include <TOE/Event/Events.h>

#include <spdlog/spdlog.h>

namespace TOE
{
	void Window::CreateNewWindow(const WindowData& data)
	{
		// Initialize glfw
		if (!WasGLFWInit)
		{
			glfwInit();
			glfwSetErrorCallback([](int error_code, const char* description)
				{
					spdlog::error("GLFW error {0}: {1}", error_code, description);
				});
			WasGLFWInit = true;
		}

		// Create the glfw window and set it as the current OpenGL context
		m_Data = data;
		m_NativeWindow = glfwCreateWindow(m_Data.Width, m_Data.Height, m_Data.Title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_NativeWindow);
		glfwSetWindowUserPointer(m_NativeWindow, &m_Data);

		// Initialize glew
		if (!WasGLEWInit)
		{
			glewInit();
			WasGLEWInit = true;
		}

		// Set glfw callbacks 
		
		// Window callbacks
		glfwSetWindowCloseCallback(m_NativeWindow, [](GLFWwindow* window)
			{
				WindowData* data = (WindowData*)glfwGetWindowUserPointer(window);
				data->EventBus->Publish(new WindowClosedEvent());
			});
		glfwSetWindowPosCallback(m_NativeWindow, [](GLFWwindow* window, int x, int y)
			{
				WindowData* data = (WindowData*)glfwGetWindowUserPointer(window);
				data->EventBus->Publish(new WindowMovedEvent(x, y));
			});
		glfwSetWindowSizeCallback(m_NativeWindow, [](GLFWwindow* window, int width, int height)
			{
				WindowData* data = (WindowData*)glfwGetWindowUserPointer(window);
				data->Width = width;
				data->Height= height;
				data->EventBus->Publish(new WindowResizedEvent(width, height));
			});

		// Keyboard callbacks
		glfwSetKeyCallback(m_NativeWindow, [](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				WindowData* data = (WindowData*)glfwGetWindowUserPointer(window);
				if (action == GLFW_PRESS)
				{
					data->EventBus->Publish(new KeyDownEvent(key));
				}
				else if (action == GLFW_RELEASE)
				{
					data->EventBus->Publish(new KeyUpEvent(key));
				}
			});

		// Mouse callbacks
		glfwSetCursorPosCallback(m_NativeWindow, [](GLFWwindow* window, double xpos, double ypos)
			{
				WindowData* data = (WindowData*)glfwGetWindowUserPointer(window);
				data->EventBus->Publish(new MouseMovedEvent(xpos, ypos));
			});
		glfwSetMouseButtonCallback(m_NativeWindow, [](GLFWwindow* window, int button, int action, int mods)
			{
				WindowData* data = (WindowData*)glfwGetWindowUserPointer(window);
				if (action == GLFW_PRESS)
				{
					data->EventBus->Publish(new MouseButtonDownEvent(button));
				}
				else if (action == GLFW_RELEASE)
				{
					data->EventBus->Publish(new MouseButtonUpEvent(button));
				}
			});
	}

	void Window::Update()
	{
		// Poll all GLFW events and display the graphics on the window
		glfwPollEvents();
		glfwSwapBuffers(m_NativeWindow);
	}

	WindowData Window::GetData()
	{
		return m_Data;
	}

	void Window::SetData(const WindowData& data)
	{
		m_Data = data;
	}

	void Window::SetEventBus(EventBus* bus)
	{
		m_Data.EventBus = bus;
	}
}