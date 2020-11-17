#pragma once
#include "Akkad/core.h"
#include "Scene.h"

#include <entt/entt.hpp>
namespace Akkad {

	class Entity {
	public:
		Entity(entt::entity handle, Scene* scene);
		Entity() {};

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
			AK_ASSERT(HasComponent<T>(), "Entity does not have the component");
			return m_Scene->m_Registry.get<T>(m_Handle);
		}

		template<typename T>
		bool HasComponent()
		{
			return m_Scene->m_Registry.has<T>(m_Handle);
		}


	private:
		entt::entity m_Handle;
		Scene* m_Scene = nullptr;
	};
}