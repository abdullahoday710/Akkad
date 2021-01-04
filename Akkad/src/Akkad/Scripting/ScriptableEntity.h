#pragma once
#include "Akkad/ECS/Entity.h"
#include "Akkad/Logging.h"

#include <map>
namespace Akkad {

	class ScriptableEntity
	{
	public:

		template<typename T>
		T& GetComponent()
		{
			return m_Entity.GetComponent<T>();
		}

		virtual void OnStart() {}
		virtual void OnUpdate() {}

	private:
		Entity m_Entity;
		friend class Scene;

	};
}