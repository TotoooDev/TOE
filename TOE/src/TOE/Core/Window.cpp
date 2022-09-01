#include <TOE/Core/Window.h>
#include <TOE/Event/Events.h>
#include <TOE/Debug/Instrumentor.h>

#include <spdlog/spdlog.h>

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
		SetCallbacks();

		// ImGui setup
		{
			TOE_PROFILE_SCOPE("Window::CreateNewWindow::ImGuiSetup");

			IMGUI_CHECKVERSION();
			ImGui::CreateContext();

			ImGuiIO& io = ImGui::GetIO(); (void)io;
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
			io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
			//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
			// io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

			// Setup Dear ImGui style
			ImGui::StyleColorsDark();
			// ImGui::StyleColorsLight();

			// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
			ImGuiStyle& style = ImGui::GetStyle();
			if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
			{
				style.WindowRounding = 0.0f;
				style.Colors[ImGuiCol_WindowBg].w = 1.0f;
			}

			// Setup Platform/Renderer backends
			ImGui_ImplGlfw_InitForOpenGL(m_NativeWindow, true);
			ImGui_ImplOpenGL3_Init("#version 330 core");
		}
	}

	void Window::Update()
	{
		TOE_PROFILE_FUNCTION();

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

	void Window::SetCallbacks()
	{
		TOE_PROFILE_FUNCTION();

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
	}
}