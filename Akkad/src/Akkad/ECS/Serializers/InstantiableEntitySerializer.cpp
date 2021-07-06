#include "InstantiableEntitySerializer.h"

#include <Akkad/ECS/Serializers/SceneSerializer.h>
#include <Akkad/ECS/Components/Components.h>

#include <fstream>
#include <iomanip>

namespace Akkad {

	void InstantiableEntitySerializer::Serialize(Entity entity, std::string outPath)
	{
		nlohmann::ordered_json data;
		auto& tag = entity.GetComponent<TagComponent>();
		data["Scene"]["Name"] = tag.Tag;
		SceneSerializer::SerializeEntity(entity, "", data);
		std::ofstream output;
		output.open(outPath, std::ios::trunc);
		output << std::setw(4) << data << std::endl;
		output.close();
	}

	void InstantiableEntitySerializer::SerializeEmpty(std::string outPath)
	{
		nlohmann::ordered_json data;
		data["Scene"]["Name"] = "Empty";

		std::ofstream output;
		output.open(outPath, std::ios::trunc);
		output << std::setw(4) << data << std::endl;
		output.close();
	}

	nlohmann::ordered_json InstantiableEntitySerializer::Deserialize(std::string filePath)
	{
		nlohmann::ordered_json data;
		std::ifstream file;
		file.open(filePath);

		file >> data;

		return data;
	}
}