#include "pch.h"
#include <TOE/Graphics/Renderer.h>
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

	void Renderer::DrawModel(const glm::mat4& transform, const Ref<Model>& model, const std::vector<Material>& materials)
	{
		for (unsigned int i = 0; i < model->GetMeshes().size(); i++)
		{
			DrawMesh(transform, model->GetMeshes()[i], materials[i]);
		}
	}

	void Renderer::DrawModel(const glm::mat4& transform, const Ref<Model>& model, const glm::vec3& color)
	{
		for (unsigned int i = 0; i < model->GetMeshes().size(); i++)
		{
			DrawMesh(transform, model->GetMeshes()[i], color);
		}
	}

	void Renderer::DrawMesh(const glm::mat4& transform, const Mesh& mesh, const Material& material)
	{
		auto vao = mesh.GetVAO();
		auto ebo = mesh.GetEBO();

		if (material.Diffuse)
		{
			m_ShaderTexture.Use();
			m_ShaderTexture.SetMat4("uModel", transform);
			m_ShaderTexture.SetInt("uTexture", 0);
		}
		else
		{
			m_ShaderColor.Use();
			m_ShaderColor.SetMat4("uModel", transform);
			m_ShaderColor.SetVec3("uColor", glm::vec3(1.0f));
		}

		vao->Use();
		ebo->Use();

		glDrawElements(GL_TRIANGLES, ebo->GetCount(), GL_UNSIGNED_INT, 0);

		m_Stats.DrawCalls++;
		m_Stats.VertexCount += vao->GetVertexCount();
		m_Stats.IndexCount += ebo->GetCount();
	}

	void Renderer::DrawMesh(const glm::mat4& transform, const Mesh& mesh, const glm::vec3& color)
	{
		auto vao = mesh.GetVAO();
		auto ebo = mesh.GetEBO();

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

	void Renderer::DrawVertexObject(const glm::mat4& transform, const Ref<VAO>& vao, const Ref<EBO>& ebo, const Ref<Texture2D>& texture)
	{
		TOE_PROFILE_FUNCTION();

		m_ShaderTexture.Use();
		m_ShaderTexture.SetMat4("uModel", transform);
		m_ShaderTexture.SetInt("uTexture", 0);

		texture->Use();

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