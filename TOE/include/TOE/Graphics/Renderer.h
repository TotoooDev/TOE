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

	// TODO: Make this a singletone with a -> operator

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

		static void Init();
		static void Recompile();

		static void Begin();
		static void End();

		static void SetTargetFramebuffer(Ref<Framebuffer> target);
		static void SetClearColor(float r, float g, float b);
		static void Clear();

		static void SetCurrentCamera(const Camera& camera, const glm::vec3& pos);
		static void SetLight(const glm::vec3& pos, const glm::vec3& rotation, const Light& light);

		static void DrawModel(const glm::mat4& transform, const Ref<Model>& model, const std::vector<Material>& materials);

		static void DrawMesh(const glm::mat4& transform, const Mesh& mesh, const std::vector<Material>& materials);

		static void DrawVertexObject(const glm::mat4& transform, const Ref<VAO>& vao, const Ref<EBO>& ebo, const Material& material);

		static Stats GetStats();

	private:
		// Move this to Primitives.cpp
		static void GenSphere();
		static void SetLight(const Shader& shader, const Light& light, const glm::vec3& pos, const glm::vec3& rotation);
		static void UpdateStats(unsigned int vertexCount, unsigned int indexCount);

		inline static Shader m_ShaderTexture;

		inline static unsigned int m_NumLights = 0;

		inline static Stats m_Stats;
		inline static Timer m_Timer;

		// Deffered lighting stuff
		inline static Shader m_ShaderGBuffer;
		inline static Shader m_ShaderLighting;
		// Framebuffer
		inline static unsigned int m_gBuffer;
		inline static unsigned int m_PosTexture;
		inline static unsigned int m_NormalTexture;
		inline static unsigned int m_AlbedoTexture;
		inline static unsigned int m_DepthTexture;
		// Target
		inline static unsigned int m_Target = 0;
		// Quad we are rendering to
		inline static unsigned int m_QuadVAO;
		inline static unsigned int m_QuadVBO;
		// Light sphere
		inline static unsigned int m_SphereVAO;
		inline static unsigned int m_SphereVBO;
		inline static unsigned int m_SphereEBO;
		inline static unsigned int m_SphereNumIndices;
	};
}