#pragma once

#include <Akkad/ECS/Entity.h>
#include <json.hpp>

namespace Akkad {
	using json = nlohmann::ordered_json;

	class GUIButtonComponentSerializer
	{
	public:
		static void Serialize(Entity entity, json& entity_data);
		static void Deserialize(Entity entity, json& component_data);

	};
}

