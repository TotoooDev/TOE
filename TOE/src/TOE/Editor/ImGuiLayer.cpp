#include "pch.h"
#include <TOE/Editor/ImGuiLayer.h>
#include <TOE/Core/Application.h>

#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_glfw.h>
#include <ImGui/imgui_impl_opengl3.h>

namespace TOE
{
	ImGuiLayer::~ImGuiLayer()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::OnCreate()
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		// io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

		io.FontDefault = io.Fonts->AddFontFromFileTTF("fonts/opensans/OpenSans-Regular.ttf", 18.0f);

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		// ImGui::StyleColorsLight();
		SetDarkColors();

		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		ImGuiStyle& style = ImGui::GetStyle();
		style.TabRounding = 0.0f;
		style.TabBorderSize = 0.0f;
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 3.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		// Setup Platform/Renderer backends
		ImGui_ImplOpenGL3_Init("#version 330 core");
		ImGui_ImplGlfw_InitForOpenGL(Application::Get().GetWindow().GetNativeWindow(), true);
	}

	void ImGuiLayer::Begin()
	{
		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void ImGuiLayer::End()
	{
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		ImGuiIO& io = ImGui::GetIO(); (void)io;
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}

	void ImGuiLayer::SetDarkColors()
	{
		auto& colors = ImGui::GetStyle().Colors;
		colors[ImGuiCol_WindowBg] = ImVec4(0.071f, 0.071f, 0.071f, 1.0f);

		// Header Bar
		colors[ImGuiCol_Header] = ImVec4(0.314f, 0.314f, 0.314f, 1.0f);
		colors[ImGuiCol_HeaderHovered] = ImVec4(0.188f, 0.188f, 0.188f, 1.0f);
		colors[ImGuiCol_HeaderActive] = ImVec4(0.314f, 0.314f, 0.314f, 1.0f);

		// Buttons
		colors[ImGuiCol_Button] = ImVec4(0.314f, 0.314f, 0.314f, 1.0f);
		colors[ImGuiCol_ButtonHovered] = ImVec4(0.188f, 0.188f, 0.188f, 1.0f);
		colors[ImGuiCol_ButtonActive] = ImVec4(0.314f, 0.314f, 0.314f, 1.0f);

		// Checkboxes
		colors[ImGuiCol_CheckMark] = ImVec4(0.61, 0.61, 0.61, 1.0f);
		colors[ImGuiCol_FrameBg] = ImVec4(0.188f, 0.188f, 0.188f, 1.0f);
		colors[ImGuiCol_FrameBgHovered] = ImVec4(0.314f, 0.314f, 0.314f, 1.0f);
		colors[ImGuiCol_FrameBgActive] = ImVec4(0.314f, 0.314f, 0.314f, 1.0f);

		// Tab
		colors[ImGuiCol_Tab] = ImVec4(0.188f, 0.188f, 0.188f, 1.0f);
		colors[ImGuiCol_TabUnfocused] = ImVec4(0.188f, 0.188f, 0.188f, 1.0f);
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.314f, 0.314f, 0.314f, 1.0f);
		colors[ImGuiCol_TabActive] = ImVec4(0.314f, 0.314f, 0.314f, 1.0f);
		colors[ImGuiCol_TabHovered] = ImVec4(0.314f, 0.314f, 0.314f, 1.0f);

		// Title
		colors[ImGuiCol_TitleBg] = ImVec4(0.188f, 0.188f, 0.188f, 1.0f);
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.314f, 0.314f, 0.314f, 1.0f);

		// Resize Grip
		colors[ImGuiCol_ResizeGrip] = ImVec4(0.188f, 0.188f, 0.188f, 1.0f);
		colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.314f, 0.314f, 0.314f, 1.0f);
		colors[ImGuiCol_ResizeGripActive] = ImVec4(0.314f, 0.314f, 0.314f, 1.0f);
	}
}