#include "pch.h"
#include <TOE/Core/Window.h>
#include <TOE/Core/GlobalConfig.h>
#include <TOE/Event/Events.h>

#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_glfw.h>
#include <ImGui/imgui_impl_opengl3.h>

namespace TOE
{
	void Window::CreateNewWindow(const WindowData& data)
	{
		TOE_PROFILE_FUNCTION();

		// Initialize glfw
		if (!WasGLFWInit)
		{
			glfwInit();
			#ifdef TOE_ENABLE_DEBUG_OPENGL
				glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
				spdlog::warn("OpenGL debug context created! The application might be running slower.");
			#endif
			glfwSetErrorCallback([](int error_code, const char* description)
				{
					spdlog::error("GLFW error {0}: {1}", error_code, description);
				});
			WasGLFWInit = true;
		}

		// Create the glfw window and set it as the current OpenGL context
		if (GlobalConfig::Get()["window"]["start_maximized"])
			glfwWindowHint(GLFW_MAXIMIZED, true);
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
		SetCallbacks();
		// OpenGL debug context
		InitOpenGLDebugOutput();
	}

	void Window::Update()
	{
		TOE_PROFILE_FUNCTION();

		// Poll all GLFW events and display the graphics on the window
		glfwPollEvents();
		glfwSwapBuffers(m_NativeWindow);
	}

	void Window::LockMouse(bool lock)
	{
		m_Data.MouseLocked = lock;
		if (lock)
			glfwSetInputMode(m_NativeWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		else
			glfwSetInputMode(m_NativeWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	GLFWwindow* Window::GetNativeWindow()
	{
		return m_NativeWindow;
	}

	WindowData Window::GetData()
	{
		return m_Data;
	}

	int Window::GetKey(int key)
	{
		return glfwGetKey(m_NativeWindow, key);
	}

	int Window::GetMouseButton(int mouseButton)
	{
		return glfwGetMouseButton(m_NativeWindow, mouseButton);
	}

	void Window::SetData(const WindowData& data)
	{
		m_Data = data;
	}

	void Window::SetEventBus(EventBus* bus)
	{
		m_Data.EventBus = bus;
	}

	void Window::InitOpenGLDebugOutput()
	{
		int flags;
		glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
		if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
		{
			glEnable(GL_DEBUG_OUTPUT);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

			// Set the callback as a lambda
			// Shamelessly stolen from https://learnopengl.com/In-Practice/Debugging
			glDebugMessageCallback(
				[](GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char* message, const void* userParam)
				{
					// Ignore non-significant error/warning codes
					if (id == 131169 || id == 131185 || id == 131218 || id == 131204)
						return;

					std::stringstream sstream;
					sstream << message << " ";

					switch (severity)
					{
					case GL_DEBUG_SEVERITY_HIGH:         spdlog::error(sstream.str()); break;
					case GL_DEBUG_SEVERITY_MEDIUM:       spdlog::warn(sstream.str()); break;
					case GL_DEBUG_SEVERITY_LOW:          spdlog::warn(sstream.str()); break;
					case GL_DEBUG_SEVERITY_NOTIFICATION: spdlog::info(sstream.str()); break;
					}
				}, nullptr);

			glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
		}
		else
		{
			spdlog::warn("Unable to create a debug context!");
		}
	}

	void Window::SetCallbacks()
	{
		TOE_PROFILE_FUNCTION();

		// Window callbacks
		glfwSetWindowCloseCallback(m_NativeWindow, [](GLFWwindow* window)
			{
				WindowData* data = (WindowData*)glfwGetWindowUserPointer(window);
				data->EventBus->Publish(new WindowClosedEvent());
			});
		glfwSetWindowCloseCallback(m_NativeWindow, [](GLFWwindow* window)
			{
				WindowData* data = (WindowData*)glfwGetWindowUserPointer(window);
				data->EventBus->Publish(new WindowClosedEvent());
			});
		glfwSetWindowMaximizeCallback(m_NativeWindow, [](GLFWwindow* window, int maximized)
			{
				WindowData* data = (WindowData*)glfwGetWindowUserPointer(window);
				if (maximized)
					data->EventBus->Publish(new WindowMaximizedEvent());
				else
					data->EventBus->Publish(new WindowRestoredEvent());
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
				data->Height = height;
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
		glfwSetScrollCallback(m_NativeWindow, [](GLFWwindow* window, double xoffset, double yoffset)
			{
				WindowData* data = (WindowData*)glfwGetWindowUserPointer(window);
				data->EventBus->Publish(new MouseScrolledEvent(xoffset, yoffset));
			});
	}
}