#pragma once

#include <TOE/Core/Ref.h>
#include <TOE/Core/Timer.h>
#include <TOE/Graphics/PerspectiveCamera.h>
#include <TOE/Graphics/Shader.h>
#include <TOE/Graphics/Texture.h>
#include <TOE/Graphics/Mesh.h>
#include <TOE/Graphics/Material.h>
#include <TOE/Graphics/Light.h>
#include <TOE/Graphics/Framebuffer.h>
#include <TOE/Event/WindowEvents.h>
#include <TOE/Scene/Scene.h>

#include <glm/glm.hpp>

namespace TOE
{
	class Camera
	{
	public:
		Camera(const glm::mat4& projection, const glm::mat4& view)
			: m_Projection(projection), m_View(view) {}

		glm::mat4 GetProjection() const { return m_Projection; }
		glm::mat4 GetView() const { return m_View; }

	private:
		glm::mat4 m_Projection;
		glm::mat4 m_View;
	};

	// TODO: Make this a singleton with a -> operator

	class Renderer
	{
	public:
		struct Stats
		{
			unsigned int DrawCalls;
			unsigned int VertexCount;
			unsigned int IndexCount;
			float RenderTime;
		};

		static Renderer& Get()
		{
			static Renderer instance;
			return instance;
		}

		void Recompile();

		void Begin();
		void End();

		void SetTargetFramebuffer(Ref<Framebuffer> target);
		void SetClearColor(float r, float g, float b);
		void Clear();

		void SetCurrentCamera(const Camera& camera, const glm::vec3& pos);
		void SetLight(const glm::vec3& pos, const glm::vec3& rotation, const Light& light);

		void DrawModel(const glm::mat4& transform, const Ref<Model>& model, const std::vector<Material>& materials);

		void DrawMesh(const glm::mat4& transform, const Mesh& mesh, const std::vector<Material>& materials);

		void DrawVertexObject(const glm::mat4& transform, const Ref<VAO>& vao, const Ref<EBO>& ebo, const Material& material);

		Stats GetStats();

	private:
		Renderer();
		Renderer(const Renderer&) = delete;
		void operator=(const Renderer&) = delete;

		void SetLight(const Shader& shader, const Light& light, const glm::vec3& pos, const glm::vec3& rotation);
		void UpdateStats(unsigned int vertexCount, unsigned int indexCount);

		Shader m_ShaderTexture;

		unsigned int m_NumLights = 0;

		Stats m_Stats;
		Timer m_Timer;

		// Deffered lighting stuff
		Shader m_ShaderGBuffer;
		Shader m_ShaderLighting;
		// Framebuffer
		unsigned int m_gBuffer;
		unsigned int m_PosTexture;
		unsigned int m_NormalTexture;
		unsigned int m_AlbedoTexture;
		unsigned int m_DepthTexture;
		// Target
		unsigned int m_Target = 0;
		// Quad we are rendering to
		VAO m_QuadVAO;
		EBO m_QuadEBO;
	};
}