#pragma once

#include <TOE/Core/Core.h>
#include <TOE/Core/Application.h>
#include <TOE/Core/Ref.h>
#include <TOE/Core/GlobalConfig.h>

#include <TOE/Debug/Instrumentor.h>

#include <TOE/Editor/EditorSink.h>
#include <TOE/Editor/Panels/LogPanel.h>

#include <TOE/Event/Events.h>

#include <TOE/Graphics/Renderer.h>
#include <TOE/Graphics/Framebuffer.h>

#include <TOE/Scene/Scene.h>
#include <TOE/Scene/Entity.h>
#include <TOE/Scene/Components.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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
		void OnKeyPressed(KeyDownEvent* event);
		void OnKeyUp(KeyUpEvent* event);
		void OnWindowMaximized(WindowMaximizedEvent* event);
		void OnWindowRestored(WindowRestoredEvent* event);

		Ref<Framebuffer> m_Framebuffer;
		ImVec2 m_ViewportSize;

		Scene m_Scene;
		Entity m_Ent, m_CamEnt;

		bool m_ShowLogs;
		bool m_ShowSettings;
		LogPanel m_Console;
	};
}