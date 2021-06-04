#pragma once
#include "Akkad/core.h"
#include "Scene.h"

#include <entt/entt.hpp>
namespace Akkad {

	class Entity {
	public:
		Entity(entt::entity handle, Scene* scene);
		Entity() {};

		bool operator==(const Entity& other)
		{
			if (other.m_Handle == m_Handle)
			{
				return true;
			}
			return false;
		}

		bool operator!=(const Entity& other)
		{
			if (other.m_Handle != m_Handle)
			{
				return true;
			}
			return false;
		}


		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			AK_ASSERT(!HasComponent<T>(), "Entity already has the component !");
			T& component = m_Scene->m_Registry.emplace<T>(m_Handle, std::forward<Args>(args)...);
			return component;
		}

		template<typename T>
		T& GetComponent()
		{
			return m_Scene->m_Registry.get<T>(m_Handle);
		}

		template<typename T>
		void RemoveComponent()
		{
			m_Scene->m_Registry.remove_if_exists<T>(m_Handle);
		}

		template<typename T>
		bool HasComponent()
		{
			return m_Scene->m_Registry.has<T>(m_Handle);
		}

		bool IsValid()
		{
			if (m_Scene != nullptr)
			{
				if (m_Handle != entt::null)
				{
					if (m_Scene->m_Registry.valid(m_Handle))
					{
						return true;
					}
				}

			}
			

			return false;
		}


	private:
		entt::entity m_Handle = entt::null;
		Scene* m_Scene = nullptr;

		friend class Scene;
		friend class SceneHierarchyPanel;
	};

	struct RelationShipComponent {
		std::size_t children = 0;
		Entity first_child;
		Entity last_child;
		Entity prev;
		Entity next;
		Entity parent;
	};
}