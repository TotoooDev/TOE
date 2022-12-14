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
		// Softer Cherry style from ImThemes
		ImGuiStyle& style = ImGui::GetStyle();

		style.Alpha = 1.0;
		style.DisabledAlpha = 0.4000000059604645;
		style.WindowPadding = ImVec2(10.0, 10.0);
		style.WindowRounding = 4.0;
		style.WindowBorderSize = 0.0;
		style.WindowMinSize = ImVec2(50.0, 50.0);
		style.WindowTitleAlign = ImVec2(0.5, 0.5);
		style.WindowMenuButtonPosition = ImGuiDir_Left;
		style.ChildRounding = 0.0;
		style.ChildBorderSize = 1.0;
		style.PopupRounding = 1.0;
		style.PopupBorderSize = 1.0;
		style.FramePadding = ImVec2(5.0, 3.0);
		style.FrameRounding = 3.0;
		style.FrameBorderSize = 0.0;
		style.ItemSpacing = ImVec2(6.0, 6.0);
		style.ItemInnerSpacing = ImVec2(3.0, 2.0);
		style.CellPadding = ImVec2(3.0, 3.0);
		style.IndentSpacing = 6.0;
		style.ColumnsMinSpacing = 6.0;
		style.ScrollbarSize = 13.0;
		style.ScrollbarRounding = 16.0;
		style.GrabMinSize = 20.0;
		style.GrabRounding = 4.0;
		style.TabRounding = 4.0;
		style.TabBorderSize = 1.0;
		style.TabMinWidthForCloseButton = 0.0;
		style.ColorButtonPosition = ImGuiDir_Right;
		style.ButtonTextAlign = ImVec2(0.5, 0.5);
		style.SelectableTextAlign = ImVec2(0.0, 0.0);

		style.Colors[ImGuiCol_Text] = ImVec4(0.8588235378265381, 0.929411768913269, 0.886274516582489, 1.0);
		style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.5215686559677124, 0.5490196347236633, 0.5333333611488342, 1.0);
		style.Colors[ImGuiCol_WindowBg] = ImVec4(0.1294117718935013, 0.1372549086809158, 0.168627455830574, 1.0);
		style.Colors[ImGuiCol_ChildBg] = ImVec4(0.1490196138620377, 0.1568627506494522, 0.1882352977991104, 1.0);
		style.Colors[ImGuiCol_PopupBg] = ImVec4(0.2000000029802322, 0.2196078449487686, 0.2666666805744171, 1.0);
		style.Colors[ImGuiCol_Border] = ImVec4(0.1372549086809158, 0.1137254908680916, 0.1333333402872086, 1.0);
		style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.0, 0.0, 0.0, 1.0);
		style.Colors[ImGuiCol_FrameBg] = ImVec4(0.168627455830574, 0.1843137294054031, 0.2313725501298904, 1.0);
		style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.4549019634723663, 0.196078434586525, 0.2980392277240753, 1.0);
		style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.4549019634723663, 0.196078434586525, 0.2980392277240753, 1.0);
		style.Colors[ImGuiCol_TitleBg] = ImVec4(0.2313725501298904, 0.2000000029802322, 0.2705882489681244, 1.0);
		style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.501960813999176, 0.07450980693101883, 0.2549019753932953, 1.0);
		style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.2000000029802322, 0.2196078449487686, 0.2666666805744171, 1.0);
		style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.2000000029802322, 0.2196078449487686, 0.2666666805744171, 1.0);
		style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.239215686917305, 0.239215686917305, 0.2196078449487686, 1.0);
		style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.3882353007793427, 0.3882353007793427, 0.3725490272045135, 1.0);
		style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.6941176652908325, 0.6941176652908325, 0.686274528503418, 1.0);
		style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.6941176652908325, 0.6941176652908325, 0.686274528503418, 1.0);
		style.Colors[ImGuiCol_CheckMark] = ImVec4(0.658823549747467, 0.1372549086809158, 0.1764705926179886, 1.0);
		style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.6509804129600525, 0.1490196138620377, 0.3450980484485626, 1.0);
		style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.7098039388656616, 0.2196078449487686, 0.2666666805744171, 1.0);
		style.Colors[ImGuiCol_Button] = ImVec4(0.6509804129600525, 0.1490196138620377, 0.3450980484485626, 1.0);
		style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.4549019634723663, 0.196078434586525, 0.2980392277240753, 1.0);
		style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.4549019634723663, 0.196078434586525, 0.2980392277240753, 1.0);
		style.Colors[ImGuiCol_Header] = ImVec4(0.4549019634723663, 0.196078434586525, 0.2980392277240753, 1.0);
		style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.6509804129600525, 0.1490196138620377, 0.3450980484485626, 1.0);
		style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.501960813999176, 0.07450980693101883, 0.2549019753932953, 1.0);
		style.Colors[ImGuiCol_Separator] = ImVec4(0.4274509847164154, 0.4274509847164154, 0.4980392158031464, 1.0);
		style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.09803921729326248, 0.4000000059604645, 0.7490196228027344, 1.0);
		style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.09803921729326248, 0.4000000059604645, 0.7490196228027344, 1.0);
		style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.6509804129600525, 0.1490196138620377, 0.3450980484485626, 1.0);
		style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.4549019634723663, 0.196078434586525, 0.2980392277240753, 1.0);
		style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.4549019634723663, 0.196078434586525, 0.2980392277240753, 1.0);
		style.Colors[ImGuiCol_Tab] = ImVec4(0.4549019634723663, 0.196078434586525, 0.2980392277240753, 1.0);
		style.Colors[ImGuiCol_TabHovered] = ImVec4(0.658823549747467, 0.1490196138620377, 0.3450980484485626, 1.0);
		style.Colors[ImGuiCol_TabActive] = ImVec4(0.501960813999176, 0.07450980693101883, 0.2549019753932953, 1.0);
		style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.06666667014360428, 0.1019607856869698, 0.1450980454683304, 1.0);
		style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.06666667014360428, 0.1019607856869698, 0.1450980454683304, 1.0);
		style.Colors[ImGuiCol_PlotLines] = ImVec4(0.8588235378265381, 0.929411768913269, 0.886274516582489, 1.0);
		style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.4549019634723663, 0.196078434586525, 0.2980392277240753, 1.0);
		style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.3098039329051971, 0.7764706015586853, 0.196078434586525, 1.0);
		style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.4549019634723663, 0.196078434586525, 0.2980392277240753, 1.0);
		style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.1882352977991104, 0.1882352977991104, 0.2000000029802322, 1.0);
		style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.3098039329051971, 0.3098039329051971, 0.3490196168422699, 1.0);
		style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.2274509817361832, 0.2274509817361832, 0.2470588237047195, 1.0);
		style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.1294117718935013, 0.1372549086809158, 0.168627455830574, 1.0);
		style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(0.214010015130043, 0.2276521772146225, 0.2784313857555389, 1.0);
		style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.3843137323856354, 0.6274510025978088, 0.9176470637321472, 1.0);
		style.Colors[ImGuiCol_DragDropTarget] = ImVec4(1.0, 1.0, 0.0, 1.0);
		style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.2588235437870026, 0.5882353186607361, 0.9764705896377563, 1.0);
		style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.0, 1.0, 1.0, 1.0);
		style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.800000011920929, 0.800000011920929, 0.800000011920929, 1.0);
		style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.800000011920929, 0.800000011920929, 0.800000011920929, 0.300000011920929);
	}
}