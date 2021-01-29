#pragma once
#include "GameAssembly.h"

#include <Akkad/Logging.h>
#include <Akkad/Scripting/ScriptableEntity.h>

#include <map>
#include <functional>
namespace Akkad {

	class ScriptFactory
	{
	public:
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

		};

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
		}

		std::map<std::string, std::function<ScriptableEntity* ()>> objectmap;

	private:
		ScriptFactory() {};
		~ScriptFactory() {};

		template<class ObjectType>
		static ScriptableEntity* instantiate() {
			return new ObjectType();
		}

		friend class GameAssembly;
	};

}


