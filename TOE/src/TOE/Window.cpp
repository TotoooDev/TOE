#include <TOE/Window.h>
#include <TOE/Event/WindowEvents.h>

namespace TOE
{
	Window::Window()
	{

	}

	Window::~Window()
	{

	}

	void Window::CreateNewWindow(const WindowData& data)
	{
		if (!WasGLFWInit)
		{
			glfwInit();
			WasGLFWInit = true;
		}

		m_Data = data;
		m_NativeWindow = glfwCreateWindow(m_Data.Width, m_Data.Height, m_Data.Title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_NativeWindow);
		glfwSetWindowUserPointer(m_NativeWindow, &m_Data);

		if (!WasGLEWInit)
		{
			glewInit();
			WasGLEWInit = true;
		}

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
				data->EventBus->Publish(new WindowResizedEvent(width, height));
			});
	}

	void Window::Update()
	{
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