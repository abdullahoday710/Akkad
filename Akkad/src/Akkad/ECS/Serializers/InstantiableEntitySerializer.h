#pragma once
#include <Akkad/core.h>
#include <Akkad/ECS/Entity.h>

#include <json.hpp>

#include <string>
namespace Akkad {

	class InstantiableEntitySerializer
	{
	public:
		static void Serialize(Entity entity, std::string outPath);
		static void SerializeEmpty(std::string outPath);

		static nlohmann::ordered_json Deserialize(std::string filePath);
	};
}
