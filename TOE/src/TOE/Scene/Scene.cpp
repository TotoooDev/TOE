#include "pch.h"
#include <TOE/Scene/Scene.h>
#include <TOE/Scene/Entity.h>
#include <TOE/Scene/Components.h>
#include <TOE/Core/Core.h>
#include <TOE/Core/Application.h>
#include <TOE/Graphics/Renderer.h>

#include <glm/gtc/matrix_transform.hpp>

namespace TOE
{
	Entity Scene::CreateEntity(const std::string& tag)
	{
		TOE_PROFILE_FUNCTION();

		Application::Get().EventBus.Subscribe(this, &Scene::OnWindowResized);

		auto ent = m_Registry.create();
		Entity entity(ent, this);

		auto& tagComp = entity.AddComponent<TagComponent>();
		if (tag.empty())
			tagComp.Tag = "Unnamed Entity";
		else
			tagComp.Tag = tag;
		
		entity.AddComponent<TransformComponent>(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));

		return entity;
	}

	void Scene::DeleteEntity(Entity entity)
	{
		TOE_PROFILE_FUNCTION();
		
		TOE_ASSERT(entity.IsValid(), "Entity does not exist!");
		m_Registry.destroy(entity.m_Identifier);
	}

	void Scene::UpdateEditor(double timestep, Ref<EditorCamera> camera)
	{
		Renderer::SetClearColor(0.1f, 0.1f, 0.1f);
		Renderer::Clear();
		Renderer::SetCurrentCamera(Camera(camera->GetProjection(), camera->GetView()));

		// Loop through all renderable entities
		{
			TOE_PROFILE_SCOPE("Scene::Update::RenderEntities");
			auto view = m_Registry.view<TransformComponent, MeshComponent>();
			for (auto&& [entity, transform, mesh] : view.each())
			{
				auto ent = Entity(entity, this);

				if (!mesh.Render)
					continue;

				if (ent.HasComponent<MaterialComponent>())
				{
					auto& material = ent.GetComponent<MaterialComponent>();
					if (material.Albedo && !material.UseColor)
						Renderer::DrawVertexObject(transform.GetTransfrom(), mesh.VertexArray, mesh.ElementBuffer, material.Albedo);
					else
					{
						Renderer::DrawVertexObject(transform.GetTransfrom(), mesh.VertexArray, mesh.ElementBuffer, material.AlbedoColor);
					}
				}
				else
				{
					Renderer::DrawVertexObject(transform.GetTransfrom(), mesh.VertexArray, mesh.ElementBuffer, glm::vec3(1.0f));
				}

			}
		}
	}

	void Scene::UpdateRuntime(double timestep)
	{
		TOE_PROFILE_FUNCTION();

		Renderer::SetClearColor(0.1f, 0.1f, 0.1f);
		Renderer::Clear();

		// Loop through all the camera components
		{
			TOE_PROFILE_SCOPE("Scene::Update::Cameras");
			auto view = m_Registry.view<TransformComponent, CameraComponent>();
			for (auto&& [entity, transform, camera] : view.each())
			{
				if (camera.Primary)
				{
					const auto& cam = camera.Cam;
					cam->UpdateCameraVectors();
					glm::vec3 pos = transform.Translation;
					glm::mat4 view = glm::lookAt(pos, pos + cam->Front, cam->Up);
					glm::mat4 projection = glm::perspective(cam->FOV, cam->ViewportWidth / cam->ViewportHeight, cam->Near, cam->Far);

					Renderer::SetCurrentCamera(Camera(projection, view));
				}
			}
		}

		// Loop through all renderable entities
		{
			TOE_PROFILE_SCOPE("Scene::Update::RenderEntities");
			auto view = m_Registry.view<TransformComponent, MeshComponent>();
			for (auto&& [entity, transform, mesh] : view.each())
			{
				auto ent = Entity(entity, this);

				if (!mesh.Render)
					continue;

				if (ent.HasComponent<MaterialComponent>())
				{
					auto& material = ent.GetComponent<MaterialComponent>();
					if (material.Albedo)
						Renderer::DrawVertexObject(transform.GetTransfrom(), mesh.VertexArray, mesh.ElementBuffer, material.Albedo);
					else
					{
						Renderer::DrawVertexObject(transform.GetTransfrom(), mesh.VertexArray, mesh.ElementBuffer, material.AlbedoColor);
					}
				}
				else
				{
					Renderer::DrawVertexObject(transform.GetTransfrom(), mesh.VertexArray, mesh.ElementBuffer, glm::vec3(1.0f));
				}

			}
		}
	}

	void Scene::OnViewportResize(unsigned int width, unsigned int height)
	{
		auto view = m_Registry.view<CameraComponent>();
		for (auto&& [entity, camComponent] : view.each())
		{
			if (!camComponent.FixedAspectRatio)
			{
				camComponent.Cam->ViewportWidth = (float)width;
				camComponent.Cam->ViewportHeight = (float)height;
				camComponent.Cam->UpdateCameraVectors();
			}
		}
	}

	void Scene::OnWindowResized(WindowResizedEvent* event)
	{
		auto view = m_Registry.view<CameraComponent>();
		for (auto&& [entity, camera] : view.each())
		{
			camera.Cam->ViewportWidth =(float) event->Width;
			camera.Cam->ViewportHeight = (float)event->Height;
		}
	}
}