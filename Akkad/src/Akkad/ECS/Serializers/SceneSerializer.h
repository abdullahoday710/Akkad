#pragma once
#include <Akkad/core.h>

#include "Akkad/ECS/Scene.h"

#include <json.hpp>

namespace Akkad {
	class SceneSerializer
	{
	public:
		static void Serialize(SharedPtr<Scene> scene, std::string& outputPath);
		static void Deserialize(SharedPtr<Scene> scene, std::string filepath);
		static void SerializeEntity(Entity entity, std::string parent_id, nlohmann::ordered_json& data);
		static void DeserializeEntity(Entity entity, std::string entity_key, Scene* scene, nlohmann::ordered_json& data);

	};
}