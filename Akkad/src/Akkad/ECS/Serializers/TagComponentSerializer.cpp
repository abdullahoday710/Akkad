#include "TagComponentSerializer.h"

#include "Akkad/ECS/Components/TagComponent.h"
namespace Akkad {

	void TagComponentSerializer::Serialize(Entity entity, json& entity_data)
	{
		auto& tag = entity.GetComponent<TagComponent>().Tag;
		entity_data["Tag"] = tag;
	}

	void TagComponentSerializer::Deserialize(Entity entity, json& component_data)
	{
		auto& tag = entity.GetComponent<TagComponent>();
		std::string tagstr = component_data;
		tag.Tag = tagstr;
	}

}