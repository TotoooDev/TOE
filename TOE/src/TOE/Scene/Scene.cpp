#include <TOE/Scene/Scene.h>
#include <TOE/Scene/Entity.h>
#include <TOE/Scene/Components.h>
#include <TOE/Core/Core.h>
#include <TOE/Graphics/Renderer.h>

#include <glm/gtc/matrix_transform.hpp>

namespace TOE
{
	Entity Scene::CreateEntity(std::string tag)
	{
		auto ent = m_Registry.create();
		Entity entity(ent, this);

		auto& tagComp = entity.AddComponent<TagComponent>();
		if (tag.empty())
			tagComp.Tag = "Unnamed Entity";
		else
			tagComp.Tag = tag;
		
		entity.AddComponent<TransformComponent>();

		return entity;
	}

	void Scene::DeleteEntity(Entity entity)
	{
		TOE_ASSERT(entity.IsValid(), "Entity does not exist!");
		m_Registry.destroy(entity.m_Identifier);
	}

	void Scene::Update(double timestep)
	{
		Renderer::Clear();

		// Loop through all the camera components
		{
			auto view = m_Registry.view<TransformComponent, CameraComponent>();
			for (auto&& [entity, transform, camera] : view.each())
			{
				if (camera.Primary)
				{
					PerspectiveCamera cam = camera.Cam;
					glm::vec3 pos = transform.Transform[3]; // Get the position from the matrix
					glm::mat4 view = glm::lookAt(pos, pos + cam.Front, cam.Up);
					glm::mat4 projection = glm::perspective(cam.FOV, cam.ViewportWidth / cam.ViewportHeight, cam.Near, cam.Far);

					Renderer::SetCurrentCamera(Camera(projection, view));
				}
			}
		}

		// Loop through all renderable entities
		{
			auto view = m_Registry.view<TransformComponent, RenderComponent>();
			for (auto&& [entity, transform, render] : view.each())
			{
				if (!render.Render)
					continue;

				Renderer::DrawVertexObject(transform, render.VertexArray, render.ElementBuffer, render.Texture);
			}
		}
	}
}