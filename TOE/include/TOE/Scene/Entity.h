#pragma once

#include <TOE/Scene/Scene.h>
#include <TOE/Core/Core.h>

namespace TOE
{
	class Entity
	{
	public:
		Entity(entt::entity identifier = entt::null, Scene* scene = nullptr)
			: m_Identifier(identifier), m_Scene(scene)
		{

		}

		template <typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			TOE_ASSERT(!HasComponent<T>(), "Entity already has component!");
			return m_Scene->m_Registry.emplace<T>(m_Identifier, std::forward<Args>(args)...);
		}

		template <typename T>
		bool HasComponent()
		{
			return m_Scene->m_Registry.any_of<T>(m_Identifier);
		}

		template <typename T>
		T& GetComponent()
		{
			TOE_ASSERT(HasComponent<T>(), "Entity does not have component!");
			return m_Scene->m_Registry.get<T>(m_Identifier);
		}

		template <typename T>
		void RemoveComponent()
		{
			TOE_ASSERT(HasComponent<T>(), "Entity does not have component!");
			m_Scene->m_Registry.remove<T>(m_Identifier);
		}

		bool IsValid()
		{
			return m_Scene->m_Registry.valid(m_Identifier);
		}

	private:
		entt::entity m_Identifier = entt::null;
		Scene* m_Scene = nullptr;

		friend class Scene;
	};
}