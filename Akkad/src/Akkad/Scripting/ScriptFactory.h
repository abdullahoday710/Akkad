#pragma once
#include "ScriptableEntity.h"

#include <functional>
namespace Akkad {

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
			auto it = objectmap.find(name);

			if (it == objectmap.end())
			{
				std::function<ScriptableEntity* ()> func = instantiate<ObjectType>;
				objectmap[name] = func;
				return true;
			}
			else
			{
				AK_ASSERT(false, "Trying to register a script that already exists !");
				return false;
			}

		}

	private:
		ScriptFactory() {};
		~ScriptFactory() {};

		std::map<std::string, std::function<ScriptableEntity* ()>> objectmap;

		template<class ObjectType>
		static ScriptableEntity* instantiate() {
			return new ObjectType();
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

		friend class PropertyEditorPanel;
		friend class Scene;
	};
}