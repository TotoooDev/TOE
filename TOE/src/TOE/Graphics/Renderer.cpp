#include <TOE/Graphics/Renderer.h>
#include <TOE/Debug/Instrumentor.h>
#include <GL/glew.h>

namespace TOE
{
	void Renderer::Init()
	{
		m_ShaderTexture.LoadFromFolder("shaders/texture/", "texture");
		m_ShaderColor.LoadFromFolder("shaders/color/", "color");
	}

	void Renderer::Recompile()
	{
		m_ShaderTexture.Reload();
		m_ShaderColor.Reload();
	}

	void Renderer::SetClearColor(float r, float g, float b)
	{
		glClearColor(r, g, b, 1.0f);
	}

	void Renderer::Clear()
	{
		TOE_PROFILE_FUNCTION();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		m_Stats = { 0, 0, 0 };
	}

	void Renderer::SetCurrentCamera(const Camera& camera)
	{
		TOE_PROFILE_FUNCTION();

		m_ShaderTexture.Use();
		m_ShaderTexture.SetMat4("uView", camera.GetView());
		m_ShaderTexture.SetMat4("uProjection", camera.GetProjection());
		m_ShaderColor.Use();
		m_ShaderColor.SetMat4("uView", camera.GetView());
		m_ShaderColor.SetMat4("uProjection", camera.GetProjection());
	}

	void Renderer::DrawVertexObject(const glm::mat4& transform, const Ref<VAO>& vao, const Ref<EBO>& ebo, const glm::vec3& color)
	{
		TOE_PROFILE_FUNCTION();

		m_ShaderColor.Use();
		m_ShaderColor.SetMat4("uModel", transform);
		m_ShaderColor.SetVec3("uColor", color);

		vao->Use();
		ebo->Use();

		glDrawElements(GL_TRIANGLES, ebo->GetCount(), GL_UNSIGNED_INT, 0);

		m_Stats.DrawCalls++;
		m_Stats.VertexCount += vao->GetVertexCount();
		m_Stats.IndexCount += ebo->GetCount();
	}

	Renderer::Stats Renderer::GetStats()
	{
		return m_Stats;
	}
}