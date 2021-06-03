#include "GUITextComponentSerializer.h"

#include "Akkad/Application/Application.h"
#include "Akkad/Asset/AssetManager.h"
#include "Akkad/ECS/Components/GUITextComponent.h"
namespace Akkad {
	void GUITextComponentSerializer::Serialize(Entity entity, json& entity_data)
	{
		auto& uitext = entity.GetComponent<GUITextComponent>();

		entity_data["GUITextComponent"]["Text"] = uitext.text;
		entity_data["GUITextComponent"]["FontSize"] = uitext.fontSize;
		entity_data["GUITextComponent"]["FontAssetID"] = uitext.fontAssetID;
		entity_data["GUITextComponent"]["Color"][0] = uitext.color.r;
		entity_data["GUITextComponent"]["Color"][1] = uitext.color.g;
		entity_data["GUITextComponent"]["Color"][2] = uitext.color.b;
	}

	void GUITextComponentSerializer::Deserialize(Entity entity, json& component_data)
	{
		std::string text;
		unsigned int fontSize;
		std::string fontAssetID;
		glm::vec3 color;

		text = component_data["Text"];
		fontSize = component_data["FontSize"];
		fontAssetID = component_data["FontAssetID"];

		color.r = component_data["Color"][0];
		color.g = component_data["Color"][1];
		color.b = component_data["Color"][2];


		auto& uitext = entity.AddComponent<GUITextComponent>();

		uitext.color = color;
		uitext.fontAssetID = fontAssetID;
		uitext.text = text;
		uitext.fontSize = fontSize;
	}

}