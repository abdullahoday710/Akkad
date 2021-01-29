#pragma once

#include "Akkad/Logging.h"
#include "Akkad/ECS/Entity.h"
#include "Akkad/Scripting/ScriptableEntity.h"

namespace Akkad {

	struct ScriptComponent
	{
		ScriptComponent(std::string name) { ScriptName = name; }
		std::string ScriptName;

		ScriptableEntity* Instance = nullptr;
	};

}