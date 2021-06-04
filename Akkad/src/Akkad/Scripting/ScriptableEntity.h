#pragma once
#include "Akkad/ECS/Entity.h"
#include "Akkad/Logging.h"

#include <stdexcept>

namespace Akkad {

	class ScriptableEntity
	{
	public:

		template<typename T>
		T& GetComponent()
		{
			if (m_Entity.HasComponent<T>())
			{
				return m_Entity.GetComponent<T>();
			}

			else
			{
				std::string msg = "Entity does not have the component : " + std::string(typeid(T).name());
				throw std::exception(msg.c_str());
			}
		}

		virtual void OnStart() {}
		virtual void OnUpdate() {}

	private:
		Entity m_Entity;
		friend class Scene;

	};
}