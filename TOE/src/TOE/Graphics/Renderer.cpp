#include <TOE/Graphics/Renderer.h>
#include <GL/glew.h>

namespace TOE
{
	void Renderer::Init()
	{
		m_Shader.LoadFromFolder("shaders/shader/", "shader");
	}

	void Renderer::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		m_NumDrawCalls = 0;
	}

	void Renderer::SetCurrentCamera(const Camera& camera)
	{
		m_Shader.Use();
		m_Shader.SetMat4("uView", camera.GetView());
		m_Shader.SetMat4("uProjection", camera.GetProjection());
	}

	void Renderer::DrawVertexObject(const glm::mat4& transform, VAO vao, EBO ebo, Texture2D texture)
	{
		m_Shader.Use();
		m_Shader.SetMat4("uModel", transform);

		texture.Use(0);

		vao.Use();
		ebo.Use();

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		m_NumDrawCalls++;
	}

	unsigned int Renderer::GetNumDrawCalls()
	{
		return m_NumDrawCalls;
	}
}