#include "GUIButtonComponentSerializer.h"

#include "Akkad/ECS/Components/GUIButtonComponent.h"
namespace Akkad {

	void GUIButtonComponentSerializer::Serialize(Entity entity, json& entity_data)
	{
		auto& uibutton = entity.GetComponent<GUIButtonComponent>();
		entity_data["GUIButtonComponent"]["Color"][0] = uibutton.button.GetColor().r;
		entity_data["GUIButtonComponent"]["Color"][1] = uibutton.button.GetColor().g;
		entity_data["GUIButtonComponent"]["Color"][2] = uibutton.button.GetColor().b;
	}

	void GUIButtonComponentSerializer::Deserialize(Entity entity, json& component_data)
	{
		glm::vec3 color;
		color.r = component_data["Color"][0];
		color.g = component_data["Color"][1];
		color.b = component_data["Color"][2];

		auto& uibutton = entity.AddComponent<GUIButtonComponent>();

		uibutton.button.SetColor(color);
	}

}