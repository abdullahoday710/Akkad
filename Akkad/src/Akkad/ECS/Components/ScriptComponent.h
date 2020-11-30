#pragma once

#include "Akkad/Logging.h"
#include "Akkad/ECS/Entity.h"

#include <functional>

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

	class ScriptFactory
	{
	public:

		static ScriptFactory& GetInstance()
		{ 
			static ScriptFactory instance;

			return instance;
		}

		template<class ObjectType>
		bool RegisterScriptObject(std::string name) 
		{
			std::function<ScriptableEntity* ()> func = instantiate<ObjectType>;
			objectmap[name] = func;
			return true;
		}

		ScriptableEntity* createObject(const std::string& name)
		{
			auto it = objectmap.find(name);

			if (it == objectmap.end())
			{
				AK_ERROR("Unable to instantiate script '{}' maybe it is not registered ?", name);
				return nullptr;
			}

			else
			{
				// call the instantiate function
				return it->second();
			}
		};

	private:
		ScriptFactory() {};
		~ScriptFactory() {};

		std::map<std::string, std::function<ScriptableEntity* ()>> objectmap;

		template<class ObjectType>
		static ScriptableEntity* instantiate() {
			return new ObjectType();
		}

		friend class PropertyEditorPanel;
	};

	struct ScriptComponent
	{
		ScriptComponent(std::string name) { ScriptName = name; }
		std::string ScriptName;

		ScriptableEntity* Instance = nullptr;
	};

}