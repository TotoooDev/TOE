#pragma once

#include <TOE/Core/VertexObjects.h>
#include <TOE/Core/Camera.h>
#include <TOE/Graphics/Shader.h>
#include <TOE/Graphics/Texture.h>
#include <TOE/Event/WindowEvents.h>
#include <TOE/Scene/Scene.h>

#include <glm/glm.hpp>

// Todo: Make the renderer a friend class of everything that can be drawn like meshes and stuff

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

		static void Clear();
		static void SetCurrentCamera(const Camera& camera);

		// TODO: implement BeginScene method or something
		static void DrawVertexObject(const glm::mat4& transform, VAO vao, EBO ebo, Texture2D texture);

		static Stats GetStats();

	private:
		inline static Shader m_Shader;

		inline static Stats m_Stats;
	};
}