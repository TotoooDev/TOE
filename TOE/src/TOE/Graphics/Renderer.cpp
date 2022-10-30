#include "pch.h"
#include <TOE/Graphics/Renderer.h>
#include <GL/glew.h>

namespace TOE
{
	Renderer::Renderer()
	{
		// Compile shaders
		m_ShaderTexture.LoadFromFolder("shaders/texture/", "texture");
		m_ShaderGBuffer.LoadFromFolder("shaders/gBuffer/", "gBuffer");
		m_ShaderLighting.LoadFromFolder("shaders/lighting/", "lighting");

		// Deferred shading stuff
		// Setup plane VAO
		std::vector<float> quadVertices =
		{
			// Pos              // Tex coords
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f
		};
		std::vector<unsigned int> quadIndices =
		{
			0, 1, 2,
			0, 2, 3
		};
		VertexLayout layout;
		layout.AddAttribute(Type::Float, 3); // Pos
		layout.AddAttribute(Type::Float, 2); // Tex coords
		m_QuadVAO.SetData(quadVertices, layout);
		m_QuadEBO.SetData(quadIndices);
		
		// Create framebuffer
		FramebufferSpecification spec;
		spec.Width = 1280;
		spec.Height = 720;
		spec.Resizable = true;
		spec.AddTexture(FramebufferTexture::RGBA16F);
		spec.AddTexture(FramebufferTexture::RGBA16F);
		spec.AddTexture(FramebufferTexture::RGBA8);
		spec.AddTexture(FramebufferTexture::Depth24Stencil8);

		m_gBuffer.SetSpecification(spec);
	}

	void Renderer::Recompile()
	{
		m_ShaderTexture.Reload();
		m_ShaderGBuffer.Reload();
		m_ShaderLighting.Reload();
	}

	void Renderer::Begin()
	{
		m_Stats = { 0, 0, 0, 0 };

		glEnable(GL_DEPTH_TEST);

		m_ShaderTexture.Bind();
		m_ShaderTexture.SetInt("uNumLights", m_NumLights);
		m_ShaderLighting.Bind();
		m_ShaderLighting.SetInt("uNumLights", m_NumLights);
	}

	void Renderer::End()
	{
		glDisable(GL_DEPTH_TEST);

		m_Target->Bind();

		m_gBuffer.BindAllTextures();

		m_ShaderLighting.Bind();
		m_ShaderLighting.SetInt("ugPosition", 0);
		m_ShaderLighting.SetInt("ugNormal", 1);
		m_ShaderLighting.SetInt("ugAlbedoSpec", 2);

		m_QuadVAO.Bind();
		m_QuadEBO.Bind();
		glDrawElements(GL_TRIANGLES, m_QuadEBO.GetCount(), GL_UNSIGNED_INT, 0);

		m_NumLights = 0;
		m_Timer.Reset();
	}

	void Renderer::OnViewportResize(unsigned int width, unsigned int height)
	{
		m_gBuffer.Resize(width, height);
	}

	void Renderer::SetTargetFramebuffer(Framebuffer* target)
	{
		m_Target = target;
	}

	void Renderer::SetClearColor(float r, float g, float b)
	{
		glClearColor(r, g, b, 1.0f);
	}

	void Renderer::Clear()
	{
		TOE_PROFILE_FUNCTION();

		m_Target->Bind();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		m_gBuffer.Bind();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		m_gBuffer.Unbind();
	}

	void Renderer::SetCurrentCamera(const Camera& camera, const glm::vec3& pos)
	{
		TOE_PROFILE_FUNCTION();

		m_ShaderTexture.Bind();
		m_ShaderTexture.SetMat4("uView", camera.GetView());
		m_ShaderTexture.SetMat4("uProjection", camera.GetProjection());
		m_ShaderTexture.SetVec3("uCameraPos", pos);

		m_ShaderGBuffer.Bind();
		m_ShaderGBuffer.SetMat4("uView", camera.GetView());
		m_ShaderGBuffer.SetMat4("uProjection", camera.GetProjection());
		m_ShaderGBuffer.SetVec3("uCameraPos", pos);

		m_ShaderLighting.Bind();
		m_ShaderLighting.SetVec3("uCameraPos", pos);
	}

	void Renderer::SetLight(const glm::vec3& pos, const glm::vec3& rotation, const Light& light)
	{
		SetLight(m_ShaderTexture, light, pos, rotation);
		SetLight(m_ShaderLighting, light, pos, rotation);
		m_NumLights++;
	}

	void Renderer::DrawModel(const glm::mat4& transform, const Ref<Model>& model, const std::vector<Material>& materials)
	{
		for (unsigned int i = 0; i < model->GetMeshes().size(); i++)
		{
			DrawMesh(transform, model->GetMeshes()[i], materials);
		}
	}

	void Renderer::DrawMesh(const glm::mat4& transform, const Mesh& mesh, const std::vector<Material>& materials)
	{
		auto& material = materials[mesh.GetMaterialIndex()];
		DrawVertexObject(transform, mesh.GetVAO(), mesh.GetEBO(), material);
	}

	void Renderer::DrawVertexObject(const glm::mat4& transform, const Ref<VAO>& vao, const Ref<EBO>& ebo, const Material& material)
	{
		TOE_PROFILE_FUNCTION();

		m_gBuffer.Bind();

		m_ShaderGBuffer.Bind();
		m_ShaderGBuffer.SetMat4("uModel", transform);

		unsigned int diffuseSlot = 0;
		unsigned int specularSlot = 1;
		unsigned int normalSlot = 2;

		m_ShaderGBuffer.SetInt("uMaterial.Diffuse", diffuseSlot);
		m_ShaderGBuffer.SetInt("uMaterial.Specular", specularSlot);
		m_ShaderGBuffer.SetInt("uMaterial.Normal", normalSlot);
		m_ShaderGBuffer.SetFloat("uMaterial.Shininess", material.Shininess);

		if (material.Diffuse)
			material.Diffuse->Bind(diffuseSlot);
		if (material.Specular)
			material.Specular->Bind(specularSlot);
		if (material.Normal)
			material.Normal->Bind(normalSlot);

		vao->Bind();
		ebo->Bind();

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

		// Radius calculations from https://learnopengl.com/Advanced-Lighting/Deferred-Shading
		float lightMax = std::fmaxf(std::fmaxf(light.Diffuse.r, light.Diffuse.g), light.Diffuse.b);
		float radius = (-light.Linear + std::sqrtf(light.Linear * light.Linear - 4 * light.Quadratic * (light.Constant - (256.0f / 5.0f) * lightMax))) / (2.0f * light.Quadratic);

		shader.Bind();
		shader.SetInt(lightStr + ".Type", light.Type);

		shader.SetVec3(lightStr + ".Pos", pos);
		shader.SetVec3(lightStr + ".Direction", rotation);

		shader.SetVec3(lightStr + ".Ambient", light.Ambient);
		shader.SetVec3(lightStr + ".Diffuse", light.Diffuse);
		shader.SetVec3(lightStr + ".Specular", light.Specular);

		shader.SetFloat(lightStr + ".Constant", light.Constant);
		shader.SetFloat(lightStr + ".Linear", light.Linear);
		shader.SetFloat(lightStr + ".Quadratic", light.Quadratic);
		shader.SetFloat(lightStr + ".Radius", radius);

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