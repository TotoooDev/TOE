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
		m_NumLights = 0;
		m_Timer.Reset();
		m_Stats = { 0, 0, 0, 0 };
	}

	void Renderer::SetCurrentCamera(const Camera& camera, const glm::vec3& pos)
	{
		TOE_PROFILE_FUNCTION();

		m_ShaderTexture.Use();
		m_ShaderTexture.SetMat4("uView", camera.GetView());
		m_ShaderTexture.SetMat4("uProjection", camera.GetProjection());
		m_ShaderTexture.SetVec3("uCameraPos", pos);

		m_ShaderColor.Use();
		m_ShaderColor.SetMat4("uView", camera.GetView());
		m_ShaderColor.SetMat4("uProjection", camera.GetProjection());
		m_ShaderTexture.SetVec3("uCameraPos", pos);
	}

	void Renderer::SetLight(const glm::vec3& pos, const glm::vec3& rotation, const Light& light)
	{
		SetLight(m_ShaderTexture, light, pos, rotation);
		SetLight(m_ShaderColor, light, pos, rotation);
		m_NumLights++;
		m_ShaderColor.Use();
		m_ShaderColor.SetInt("uNumLights", m_NumLights);
		m_ShaderTexture.Use();
		m_ShaderTexture.SetInt("uNumLights", m_NumLights);
	}

	void Renderer::DrawModel(const glm::mat4& transform, const Ref<Model>& model, const std::vector<Material>& materials)
	{
		for (unsigned int i = 0; i < model->GetMeshes().size(); i++)
		{
			DrawMesh(transform, model->GetMeshes()[i], materials);
		}
	}

	void Renderer::DrawModel(const glm::mat4& transform, const Ref<Model>& model, const glm::vec3& color)
	{
		for (unsigned int i = 0; i < model->GetMeshes().size(); i++)
		{
			DrawMesh(transform, model->GetMeshes()[i], color);
		}
	}

	void Renderer::DrawMesh(const glm::mat4& transform, const Mesh& mesh, const std::vector<Material>& materials)
	{
		auto material = materials[mesh.GetMaterialIndex()];
		DrawVertexObject(transform, mesh.GetVAO(), mesh.GetEBO(), material);
	}

	void Renderer::DrawMesh(const glm::mat4& transform, const Mesh& mesh, const glm::vec3& color)
	{
		DrawVertexObject(transform, mesh.GetVAO(), mesh.GetEBO(), color);
	}

	void Renderer::DrawVertexObject(const glm::mat4& transform, const Ref<VAO>& vao, const Ref<EBO>& ebo, const Material& material)
	{
		TOE_PROFILE_FUNCTION();

		m_ShaderTexture.Use();
		m_ShaderTexture.SetMat4("uModel", transform);

		unsigned int diffuseSlot = 0;
		unsigned int specularSlot = 1;
		unsigned int normalSlot = 2;

		m_ShaderTexture.SetInt("uMaterial.Diffuse", diffuseSlot);
		m_ShaderTexture.SetInt("uMaterial.Specular", specularSlot);
		m_ShaderTexture.SetInt("uMaterial.Normal", normalSlot);
		m_ShaderTexture.SetFloat("uMaterial.Shininess", material.Shininess);

		if (material.Diffuse)
			material.Diffuse->Use(diffuseSlot);
		if (material.Specular)
			material.Specular->Use(specularSlot);
		if (material.Normal)
			material.Normal->Use(normalSlot);

		vao->Use();
		ebo->Use();

		glDrawElements(GL_TRIANGLES, ebo->GetCount(), GL_UNSIGNED_INT, 0);

		UpdateStats(vao->GetVertexCount(), ebo->GetCount());
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

		UpdateStats(vao->GetVertexCount(), ebo->GetCount());
	}

	Renderer::Stats Renderer::GetStats()
	{
		return m_Stats;
	}

	void Renderer::SetLight(const Shader& shader, const Light& light, const glm::vec3& pos, const glm::vec3& rotation)
	{
		std::string lightStr = "uLights[" + std::to_string(m_NumLights) + "]";

		shader.Use();
		shader.SetInt(lightStr + ".Type", light.Type);

		shader.SetVec3(lightStr + ".Pos", pos);
		shader.SetVec3(lightStr + ".Direction", rotation);

		shader.SetVec3(lightStr + ".Ambient", light.Ambient);
		shader.SetVec3(lightStr + ".Diffuse", light.Diffuse);
		shader.SetVec3(lightStr + ".Specular", light.Specular);

		shader.SetFloat(lightStr + ".Constant", light.Constant);
		shader.SetFloat(lightStr + ".Linear", light.Linear);
		shader.SetFloat(lightStr + ".Quadratic", light.Quadratic);

		shader.SetFloat(lightStr + ".CutOff", light.CutOff);
		shader.SetFloat(lightStr + ".OuterCutOff", light.OuterCutOff);
	}

	void Renderer::UpdateStats(unsigned int vertexCount, unsigned int indexCount)
	{
		m_Stats.DrawCalls++;
		m_Stats.VertexCount += vertexCount;
		m_Stats.IndexCount += indexCount;
		m_Stats.RenderTime += m_Timer.ElapsedMillis();
	}
}