
#pragma once

#include <TOE/Core/Layer.h>
#include <TOE/Core/Ref.h>
#include <TOE/Event/Events.h>
#include <TOE/Graphics/Framebuffer.h>
#include <TOE/Scene/Scene.h>
#include <TOE/Scene/Entity.h>

#include <TOE/Editor/Panels/LogPanel.h>
#include <TOE/Editor/Panels/ScenePanel.h>
#include <TOE/Editor/Panels/RenderStatsPanel.h>
#include <TOE/Editor/Panels/ViewportPanel.h>
#include <TOE/Editor/Panels/PropertiesPanel.h>
#include <TOE/Editor/Panels/SettingsPanel.h>

#include <ImGui/imgui.h>

namespace TOE
{
	class EditorLayer : public Layer
	{
	public:
		virtual void OnCreate() override;
		virtual void OnUpdate(double timestep) override;
		virtual void OnImGuiRender() override;

	private:
		void New();
		void Save();
		void SaveAs();
		void Open();

		void OnKeyPressed(KeyDownEvent* event);
		void OnKeyUp(KeyUpEvent* event);
		void OnWindowMaximized(WindowMaximizedEvent* event);
		void OnWindowRestored(WindowRestoredEvent* event);

		Ref<Framebuffer> m_Framebuffer;
		ImVec2 m_ViewportSize;

		Ref<Scene> m_Scene;
		Entity m_Ent;

		Ref<EditorCamera> m_Camera;
		bool m_ViewportFocused = false;

		std::string m_ScenePath = "";
		bool m_OpenScene = false, m_NewScene = false;

		bool m_ShowLogsPanel;
		bool m_ShowScenePanel;
		bool m_ShowRenderStatsPanel;
		bool m_ShowViewportPanel;
		bool m_ShowPropertiesPanel;
		bool m_ShowSettingsPanel;
		bool m_ShowAboutPanel = false;
		bool m_ShowPerformanceOverlay = true;

		LogPanel m_ConsolePanel;
		ScenePanel m_ScenePanel;
		RenderStatsPanel m_StatsPanel;
		ViewportPanel m_ViewportPanel;
		PropertiesPanel m_PropertiesPanel;
		SettingsPanel m_SettingsPanel;
	};
}