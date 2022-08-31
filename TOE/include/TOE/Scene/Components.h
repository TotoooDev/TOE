#pragma once

#include <string>
#include <glm/glm.hpp>
#include <TOE/Core/Camera.h>
#include <TOE/Core/VertexObjects.h>
#include <TOE/Graphics/Texture.h>

namespace TOE
{
	struct TagComponent
	{
		std::string Tag;

		TagComponent(const std::string& tag = "")
			: Tag(tag) {}
		TagComponent(const TagComponent&) = default;
	};

	struct TransformComponent
	{
		glm::mat4 Transform;

		TransformComponent(const glm::mat4& transform = glm::mat4(1.0f))
			: Transform(transform) {}
		TransformComponent(const TransformComponent&) = default;

		operator glm::mat4& () { return Transform; }
		operator const glm::mat4& () const { return Transform; }
	};

	// To modify maybe
	struct RenderComponent
	{
		bool Render = true;
		VAO VertexArray;
		EBO ElementBuffer;
		Texture2D Texture;

		RenderComponent(const VAO& vao, const EBO& ebo, const Texture2D& texture)
			: VertexArray(vao), ElementBuffer(ebo), Texture(texture) {}
		RenderComponent(const RenderComponent&) = default;
	};

	// TODO: The camera has its own transform but it should be defined using a TransformComponent
	struct CameraComponent
	{
		bool Primary = false;
		PerspectiveCamera Cam;

		CameraComponent(const PerspectiveCamera& cam)
			: Cam(cam) {}
		CameraComponent(const CameraComponent&) = default;
	};
}