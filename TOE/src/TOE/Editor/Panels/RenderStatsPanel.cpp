#include <TOE/Editor/Panels/RenderStatsPanel.h>
#include <TOE/Graphics/Renderer.h>
#include <ImGui/imgui.h>

namespace TOE
{
	void RenderStatsPanel::Draw(bool* isOpen)
	{
		ImGui::Begin("Rederer Stats", isOpen);
		auto stats = Renderer::GetStats();
		ImGui::Text("Renderer stats:");
		ImGui::Text("Draw calls: %d", stats.DrawCalls);
		ImGui::Text("Vertex count: %d", stats.VertexCount);
		ImGui::Text("Index count: %d", stats.IndexCount);
		ImGui::Separator();
		if (ImGui::Button("Recompile shaders"))
			Renderer::Recompile();
		ImGui::End();
	}
}