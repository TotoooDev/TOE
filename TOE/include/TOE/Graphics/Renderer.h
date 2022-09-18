#pragma once

#include <TOE/Core/Ref.h>
#include <TOE/Core/Timer.h>
#include <TOE/Graphics/PerspectiveCamera.h>
#include <TOE/Graphics/Shader.h>
#include <TOE/Graphics/Texture.h>
#include <TOE/Graphics/Mesh.h>
#include <TOE/Graphics/Material.h>
#include <TOE/Graphics/Light.h>
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

		static void SetClearColor(float r, float g, float b);
		static void Clear();
		static void SetCurrentCamera(const Camera& camera, const glm::vec3& pos);
		static void SetLight(const glm::vec3& pos, const glm::vec3& rotation, const Light& light);

		static void DrawModel(const glm::mat4& transform, const Ref<Model>& model, const std::vector<Material>& materials);
		static void DrawModel(const glm::mat4& transform, const Ref<Model>& model, const glm::vec3& color);

		static void DrawMesh(const glm::mat4& transform, const Mesh& mesh, const std::vector<Material>& materials);
		static void DrawMesh(const glm::mat4& transform, const Mesh& mesh, const glm::vec3& color);

		static void DrawVertexObject(const glm::mat4& transform, const Ref<VAO>& vao, const Ref<EBO>& ebo, const Material& material);
		static void DrawVertexObject(const glm::mat4& transform, const Ref<VAO>& vao, const Ref<EBO>& ebo, const glm::vec3& color);

		static Stats GetStats();

	private:
		static void SetLight(const Shader& shader, const Light& light, const glm::vec3& pos, const glm::vec3& rotation);
		static void UpdateStats(unsigned int vertexCount, unsigned int indexCount);

		inline static Shader m_ShaderTexture;
		inline static Shader m_ShaderColor;

		inline static unsigned int m_NumLights = 0;

		inline static Stats m_Stats;
		inline static Timer m_Timer;
	};
}