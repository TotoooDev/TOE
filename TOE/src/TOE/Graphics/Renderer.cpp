#include "pch.h"
#include <TOE/Graphics/Renderer.h>
#include <GL/glew.h>

namespace TOE
{
	void Renderer::Init()
	{
		// Compile shaders
		m_ShaderTexture.LoadFromFolder("shaders/texture/", "texture");
		m_ShaderGBuffer.LoadFromFolder("shaders/gBuffer/", "gBuffer");
		m_ShaderLighting.LoadFromFolder("shaders/lighting/", "lighting");

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);

		// Deferred shading stuff
		GenSphere();
		// Setup plane VAO
		float quadVertices[] = {
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		glGenVertexArrays(1, &m_QuadVAO);
		glGenBuffers(1, &m_QuadVBO);
		glBindVertexArray(m_QuadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_QuadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		 
		// Create framebuffer
		glGenFramebuffers(1, &m_gBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, m_gBuffer);

		// Create framebuffer textures
		// Position texture
		glGenTextures(1, &m_PosTexture);
		glBindTexture(GL_TEXTURE_2D, m_PosTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, 1280, 720, 0, GL_RGBA, GL_FLOAT, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_PosTexture, 0);
		// Normal texture
		glGenTextures(1, &m_NormalTexture);
		glBindTexture(GL_TEXTURE_2D, m_NormalTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, 1280, 720, 0, GL_RGBA, GL_FLOAT, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_NormalTexture, 0);
		// Albedo + Specular texture
		glGenTextures(1, &m_AlbedoTexture);
		glBindTexture(GL_TEXTURE_2D, m_AlbedoTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1280, 720, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_AlbedoTexture, 0);
		// Depth texture
		glGenTextures(1, &m_DepthTexture);
		glBindTexture(GL_TEXTURE_2D, m_DepthTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 1280, 720, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_DepthTexture, 0);
		// Tell OpenGL which attachments we will use
		unsigned int attachments[4] = { GL_COLOR_ATTACHMENT0 , GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
		glDrawBuffers(4, attachments);
		// Don't check for errors because I'm a god programmer
	}

	void Renderer::Recompile()
	{
		m_ShaderTexture.Reload();
		m_ShaderGBuffer.Reload();
		m_ShaderLighting.Reload();
	}

	void Renderer::Begin()
	{
		m_ShaderTexture.Use();
		m_ShaderTexture.SetInt("uNumLights", m_NumLights);
		m_ShaderLighting.Use();
		m_ShaderLighting.SetInt("uNumLights", m_NumLights);
	}

	void Renderer::End()
	{

		glBindFramebuffer(GL_FRAMEBUFFER, m_Target);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_PosTexture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, m_NormalTexture);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, m_AlbedoTexture);

		m_ShaderLighting.Use();
		m_ShaderLighting.SetInt("gPosition", 0);
		m_ShaderLighting.SetInt("gNormal", 1);
		m_ShaderLighting.SetInt("gAlbedoSpec", 2);

		glBindVertexArray(m_QuadVAO);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);

		m_NumLights = 0;
		m_Timer.Reset();
		m_Stats = { 0, 0, 0, 0 };
	}

	void Renderer::SetTargetFramebuffer(Ref<Framebuffer> target)
	{
		m_Target = target->GetID();
	}

	void Renderer::SetClearColor(float r, float g, float b)
	{
		glClearColor(r, g, b, 1.0f);
	}

	void Renderer::Clear()
	{
		TOE_PROFILE_FUNCTION();

		glBindFramebuffer(GL_FRAMEBUFFER, m_Target);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glBindFramebuffer(GL_FRAMEBUFFER, m_gBuffer);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void Renderer::SetCurrentCamera(const Camera& camera, const glm::vec3& pos)
	{
		TOE_PROFILE_FUNCTION();

		m_ShaderTexture.Use();
		m_ShaderTexture.SetMat4("uView", camera.GetView());
		m_ShaderTexture.SetMat4("uProjection", camera.GetProjection());
		m_ShaderTexture.SetVec3("uCameraPos", pos);

		m_ShaderGBuffer.Use();
		m_ShaderGBuffer.SetMat4("uView", camera.GetView());
		m_ShaderGBuffer.SetMat4("uProjection", camera.GetProjection());
		m_ShaderGBuffer.SetVec3("uCameraPos", pos);

		m_ShaderLighting.Use();
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
		auto material = materials[mesh.GetMaterialIndex()];
		DrawVertexObject(transform, mesh.GetVAO(), mesh.GetEBO(), material);
	}

	void Renderer::DrawVertexObject(const glm::mat4& transform, const Ref<VAO>& vao, const Ref<EBO>& ebo, const Material& material)
	{
		TOE_PROFILE_FUNCTION();

		glBindFramebuffer(GL_FRAMEBUFFER, m_gBuffer);

		m_ShaderGBuffer.Use();
		m_ShaderGBuffer.SetMat4("uModel", transform);

		unsigned int diffuseSlot = 0;
		unsigned int specularSlot = 1;
		unsigned int normalSlot = 2;

		m_ShaderGBuffer.SetInt("uMaterial.Diffuse", diffuseSlot);
		m_ShaderGBuffer.SetInt("uMaterial.Specular", specularSlot);
		m_ShaderGBuffer.SetInt("uMaterial.Normal", normalSlot);
		m_ShaderGBuffer.SetFloat("uMaterial.Shininess", material.Shininess);

		if (material.Diffuse)
			material.Diffuse->Use(diffuseSlot);
		if (material.Specular)
			material.Specular->Use(specularSlot);
		if (material.Normal)
			material.Normal->Use(normalSlot);

		vao->Use();
		ebo->Use();

		glDrawElements(GL_TRIANGLES, ebo->GetCount(), GL_UNSIGNED_INT, 0);

		glBindVertexArray(m_SphereVAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_SphereEBO);
		glDrawElements(GL_TRIANGLES, m_SphereNumIndices, GL_UNSIGNED_INT, 0);

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

		shader.Use();
		shader.SetInt(lightStr + ".Type", light.Type);

		shader.SetVec3(lightStr + ".Pos", pos);
		shader.SetVec3(lightStr + ".Direction", rotation);

		shader.SetVec3(lightStr + ".Ambient", light.Ambient);
		shader.SetVec3(lightStr + ".Diffuse", light.Diffuse);
		shader.SetVec3(lightStr + ".Color", light.Diffuse);
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

	void Renderer::GenSphere()
	{
		// http://www.songho.ca/opengl/gl_sphere.html

		std::vector<float> vertices;
		std::vector<unsigned int> indices;

		float radius = 1.0f;
		float x, y, z, xy;
		unsigned int sectorCount = 32;
		unsigned int stackCount = 16;
		
		float sectorStep = 2 * glm::pi<float>() / sectorCount;
		float stackStep = glm::pi<float>() / stackCount;
		float sectorAngle, stackAngle;

		for (unsigned int i = 0; i <= stackCount; i++)
		{
			stackAngle = glm::pi<float>() / 2.0f - i * stackStep;
			xy = radius * std::cosf(stackAngle);
			z = radius * std::sinf(stackAngle);

			for (unsigned int j = 0; i <= sectorCount; i++)
			{
				sectorAngle = j * sectorStep;

				x = xy * std::cosf(sectorAngle);
				y = xy * std::sinf(sectorAngle);

				vertices.push_back(x);
				vertices.push_back(y);
				vertices.push_back(z);
			}
		}

		unsigned int k1, k2;
		for (unsigned int i = 0; i < stackCount; i++)
		{
			k1 = i * (sectorCount + 1);
			k2 = k1 + sectorCount + 1;

			for (unsigned int j = 0; j < sectorCount; j++, k1++, k2++)
			{
				if (i != 0)
				{
					indices.push_back(k1);
					indices.push_back(k2);
					indices.push_back(k1 + 1);
				}

				if (i != (stackCount - 1))
				{
					indices.push_back(k1 + 1);
					indices.push_back(k2);
					indices.push_back(k2 + 1);
				}
			}
		}

		glGenVertexArrays(1, &m_SphereVAO);
		glGenBuffers(1, &m_SphereVBO);
		glBindVertexArray(m_SphereVAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_SphereVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

		glGenBuffers(1, &m_SphereEBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_SphereEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), indices.data(), GL_STATIC_DRAW);

		m_SphereNumIndices = indices.size();
	}
}