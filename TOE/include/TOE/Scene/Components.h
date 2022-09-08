#pragma once

#include <TOE/Graphics/PerspectiveCamera.h>
#include <TOE/Core/Ref.h>
#include <TOE/Graphics/Texture.h>
#include <TOE/Graphics/VertexObjects.h>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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
		glm::vec3 Translation;
		glm::vec3 Rotation;
		glm::vec3 Scale;

		TransformComponent(const glm::vec3& translation, const glm::vec3& rotation, const glm::vec3& scale)
			: Translation(translation), Rotation(rotation), Scale(scale) {}
		TransformComponent(const TransformComponent&) = default;

		glm::mat4 GetTransfrom()
		{
			glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), Rotation.x, glm::vec3(1.0f, 0.0f, 0.0f)) *
								 glm::rotate(glm::mat4(1.0f), Rotation.y, glm::vec3(0.0f, 1.0f, 0.0f)) *
								 glm::rotate(glm::mat4(1.0f), Rotation.z, glm::vec3(1.0f, 0.0f, 1.0f));

			return glm::translate(glm::mat4(1.0f), Translation) *
				   rotation *
				   glm::scale(glm::mat4(1.0f), Scale);
		}
	};

	// To modify maybe
	struct RenderComponent
	{
		bool Render = true;
		bool RenderColor = false;

		Ref<VAO> VertexArray;
		Ref<EBO> ElementBuffer;
		Ref<Texture2D> Texture;
		glm::vec3 Color = glm::vec3(1.0f, 0.0f, 0.0f);

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