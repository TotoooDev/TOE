#pragma once

#include <string>
#include <glm/glm.hpp>
#include <TOE/Graphics/PerspectiveCamera.h>
#include <TOE/Core/Ref.h>
#include <TOE/Graphics/Texture.h>
#include <TOE/Graphics/VertexObjects.h>

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
		Ref<VAO> VertexArray;
		Ref<EBO> ElementBuffer;
		Ref<Texture2D> Texture;

		RenderComponent(const Ref<VAO>& vao, const Ref<EBO>& ebo, const Ref<Texture2D>& texture)
			: VertexArray(vao), ElementBuffer(ebo), Texture(texture) {}
		RenderComponent(const RenderComponent&) = default;
	};

	struct CameraComponent
	{
		bool Primary = false;
		bool FixedAspectRatio = false;
		bool OrbitingCamera = false;
		Ref<PerspectiveCamera> Cam;

		CameraComponent(const Ref<PerspectiveCamera>& cam)
			: Cam(cam) {}
		CameraComponent(const CameraComponent&) = default;
	};
}