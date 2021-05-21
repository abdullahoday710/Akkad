#include "ScriptComponentSerializer.h"

#include "Akkad/ECS/Components/ScriptComponent.h"
namespace Akkad {

	void ScriptComponentSerializer::Serialize(Entity entity, json& entity_data)
	{
		auto& script = entity.GetComponent<ScriptComponent>();
		entity_data["Script"]["Name"] = script.ScriptName;
	}

	void ScriptComponentSerializer::Deserialize(Entity entity, json& component_data)
	{
		auto& script = entity.AddComponent<ScriptComponent>(component_data["Name"]);
	}

}