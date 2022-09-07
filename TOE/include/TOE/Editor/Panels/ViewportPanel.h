#pragma once

#include <TOE/Core/Ref.h>
#include <TOE/Graphics/Framebuffer.h>
#include <TOE/Scene/Scene.h>
#include <ImGui/imgui.h>

namespace TOE
{
	class ViewportPanel
	{
	public:
		void Init(Ref<Scene> scene, Ref<Framebuffer> framebuffer, bool* focused);
		void Draw(bool* isOpen);

	private:
		Ref<Scene> m_CurrentScene;
		Ref<Framebuffer> m_Source;
		bool* m_Hovered;
		ImVec2 m_ViewportSize;
	};
}