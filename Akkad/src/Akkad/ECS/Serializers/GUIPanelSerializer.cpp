#include "GUIPanelSerializer.h"
#include "Akkad/ECS/Components/GUIPanelComponent.h"
namespace Akkad {

	void GUIPanelSerializer::Serialize(Entity entity, json& entity_data)
	{
		auto& panel = entity.GetComponent<GUIPanelComponent>();
		entity_data["GUIPanelComponent"]["Color"][0] = panel.panel.GetColor().r;
		entity_data["GUIPanelComponent"]["Color"][1] = panel.panel.GetColor().g;
		entity_data["GUIPanelComponent"]["Color"][2] = panel.panel.GetColor().b;
		entity_data["GUIPanelComponent"]["IsTransparent"] = panel.panel.IsTransparent();
	}

	void GUIPanelSerializer::Deserialize(Entity entity, json& component_data)
	{
		glm::vec3 color;
		bool transparent = component_data["IsTransparent"];
		color.r = component_data["Color"][0];
		color.g = component_data["Color"][1];
		color.b = component_data["Color"][2];

		auto& panel = entity.AddComponent<GUIPanelComponent>();

		panel.panel.SetColor(color);
		panel.panel.SetTransparent(transparent);
	}

}