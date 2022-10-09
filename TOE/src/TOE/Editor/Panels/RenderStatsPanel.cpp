#include "pch.h"
#include <TOE/Editor/Panels/RenderStatsPanel.h>
#include <TOE/Graphics/Renderer.h>
#include <ImGui/imgui.h>

namespace TOE
{
	void RenderStatsPanel::Draw(bool* isOpen)
	{
		ImGui::Begin("Rederer Stats", isOpen);
		auto stats = Renderer::Get().GetStats();
		ImGui::Text("Renderer stats:");
		ImGui::Text("Draw calls: %d", stats.DrawCalls);
		ImGui::Text("Vertex count: %d", stats.VertexCount);
		ImGui::Text("Index count: %d", stats.IndexCount);
		ImGui::Text("Render Time: %.3f ms", stats.RenderTime);
		ImGui::Separator();
		if (ImGui::Button("Recompile shaders"))
			Renderer::Get().Recompile();
		ImGui::End();
	}
}