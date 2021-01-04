#pragma once

#include "Akkad/Logging.h"
#include "Akkad/ECS/Entity.h"
#include "Akkad/Scripting/ScriptFactory.h"

namespace Akkad {

	struct ScriptComponent
	{
		ScriptComponent(std::string name) { ScriptName = name; }
		std::string ScriptName;

		ScriptableEntity* Instance = nullptr;
	};

// ---------------------------------
#define RegisterScript(type,name)\
namespace {\
	bool registerScript = ScriptFactory::GetInstance().RegisterScriptObject<type>(name);\
}
// ---------------------------------

}