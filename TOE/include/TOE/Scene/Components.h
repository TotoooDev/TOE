#pragma once

#include <TOE/Graphics/PerspectiveCamera.h>
#include <TOE/Core/Ref.h>
#include <TOE/Graphics/Texture.h>
#include <TOE/Graphics/Mesh.h>
#include <TOE/Graphics/Primitives.h>
#include <TOE/Graphics/Material.h>

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

		TransformComponent(const glm::vec3& translation = glm::vec3(0.0f), const glm::vec3& rotation = glm::vec3(0.0f), const glm::vec3& scale = glm::vec3(1.0f))
			: Translation(translation), Rotation(rotation), Scale(scale) {}
		TransformComponent(const TransformComponent&) = default;

		glm::mat4 GetTransfrom()
		{
			glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f)) *
								 glm::rotate(glm::mat4(1.0f), glm::radians(Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f)) *
								 glm::rotate(glm::mat4(1.0f), glm::radians(Rotation.z), glm::vec3(1.0f, 0.0f, 1.0f));

			return glm::translate(glm::mat4(1.0f), Translation) *
				   rotation *
				   glm::scale(glm::mat4(1.0f), Scale);
		}
	};

	struct MaterialComponent
	{
		bool UseColor = false;
		std::vector<Material> Materials;

		MaterialComponent(std::vector<Material> materials)
		: Materials(materials) {}
		MaterialComponent(const MaterialComponent&) = default;
	};

	struct MeshComponent
	{
		Ref<TOE::Model> Model;
		bool Render = true;

		MeshComponent(Ref<TOE::Model> model)
			: Model(model) {}
		MeshComponent(const MeshComponent&) = default;
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