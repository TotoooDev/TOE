#include <TOE/Graphics/Renderer.h>
#include <TOE/Debug/Instrumentor.h>
#include <GL/glew.h>

namespace TOE
{
	void Renderer::Init()
	{
		m_Shader.LoadFromFolder("shaders/shader/", "shader");
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

		m_Shader.Use();
		m_Shader.SetMat4("uView", camera.GetView());
		m_Shader.SetMat4("uProjection", camera.GetProjection());
	}

	void Renderer::DrawVertexObject(const glm::mat4& transform, const Ref<VAO>& vao, const Ref<EBO>& ebo, const Ref<Texture2D>& texture)
	{
		TOE_PROFILE_FUNCTION();

		m_Shader.Use();
		m_Shader.SetMat4("uModel", transform);

		texture->Use(0);

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