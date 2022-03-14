#include "GUICheckBoxComponentSerializer.h"

#include "Akkad/ECS/Components/GUICheckBoxComponent.h"
namespace Akkad
{
	void GUICheckBoxComponentSerializer::Serialize(Entity entity, json& entity_data)
	{
		auto& checkbox = entity.GetComponent<GUICheckBoxComponent>();
		entity_data["GUICheckBoxComponent"]["BoxColor"][0] = checkbox.box.GetBoxColor().r;
		entity_data["GUICheckBoxComponent"]["BoxColor"][1] = checkbox.box.GetBoxColor().g;
		entity_data["GUICheckBoxComponent"]["BoxColor"][2] = checkbox.box.GetBoxColor().b;

		entity_data["GUICheckBoxComponent"]["MarkColor"][0] = checkbox.box.GetMarkColor().r;
		entity_data["GUICheckBoxComponent"]["MarkColor"][1] = checkbox.box.GetMarkColor().g;
		entity_data["GUICheckBoxComponent"]["MarkColor"][2] = checkbox.box.GetMarkColor().b;
	}

	void GUICheckBoxComponentSerializer::Deserialize(Entity entity, json& component_data)
	{
		glm::vec3 boxcolor;
		boxcolor.r = component_data["BoxColor"][0];
		boxcolor.g = component_data["BoxColor"][1];
		boxcolor.b = component_data["BoxColor"][2];

		glm::vec3 markColor;
		markColor.r = component_data["MarkColor"][0];
		markColor.g = component_data["MarkColor"][1];
		markColor.b = component_data["MarkColor"][2];

		auto& checkbox = entity.AddComponent<GUICheckBoxComponent>();

		checkbox.box.SetBoxColor(boxcolor);
		checkbox.box.SetMarkColor(markColor);
	}
}

