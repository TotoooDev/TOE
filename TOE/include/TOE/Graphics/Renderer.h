#pragma once

#include <TOE/Core/Ref.h>
#include <TOE/Graphics/PerspectiveCamera.h>
#include <TOE/Graphics/Shader.h>
#include <TOE/Graphics/Texture.h>
#include <TOE/Graphics/VertexObjects.h>
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
		};

		static void Init();
		static void Recompile();

		static void SetClearColor(float r, float g, float b);
		static void Clear();
		static void SetCurrentCamera(const Camera& camera);

		// TODO: implement BeginScene method or something
		static void DrawVertexObject(const glm::mat4& transform, const Ref<VAO>& vao, const Ref<EBO>& ebo, const glm::vec3& color);

		static Stats GetStats();

	private:
		inline static Shader m_ShaderTexture;
		inline static Shader m_ShaderColor;

		inline static Stats m_Stats;
	};
}