#include "GUITextComponentSerializer.h"

#include "Akkad/Application/Application.h"
#include "Akkad/Asset/AssetManager.h"
#include "Akkad/ECS/Components/GUITextComponent.h"
namespace Akkad {
	void GUITextComponentSerializer::Serialize(Entity entity, json& entity_data)
	{
		using namespace GUI;

		auto& uitext = entity.GetComponent<GUITextComponent>();

		entity_data["GUITextComponent"]["Text"] = uitext.text;
		entity_data["GUITextComponent"]["FontSize"] = uitext.fontSize;
		entity_data["GUITextComponent"]["FontAssetID"] = uitext.fontAssetID;
		entity_data["GUITextComponent"]["Color"][0] = uitext.color.r;
		entity_data["GUITextComponent"]["Color"][1] = uitext.color.g;
		entity_data["GUITextComponent"]["Color"][2] = uitext.color.b;

		switch (uitext.alignment)
		{
		case GUIText::Alignment::LEFT:
			entity_data["GUITextComponent"]["Alignment"] = "Left";
			break;
		case GUIText::Alignment::CENTER:
			entity_data["GUITextComponent"]["Alignment"] = "Center";
			break;
		}

		switch (uitext.fittingMode)
		{
		case GUIText::FittingMode::SCALE_TO_FIT:
			entity_data["GUITextComponent"]["FittingMode"] = "SCALE_TO_FIT";
			break;
		case GUIText::FittingMode::KEEP_FONT_SIZE:
			entity_data["GUITextComponent"]["FittingMode"] = "KEEP_FONT_SIZE";
			break;
		}
	}

	void GUITextComponentSerializer::Deserialize(Entity entity, json& component_data)
	{
		using namespace GUI;

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

		if (component_data["Alignment"] == "Left")
		{
			uitext.alignment = GUIText::Alignment::LEFT;
		}
		else if (component_data["Alignment"] == "Center")
		{
			uitext.alignment = GUIText::Alignment::CENTER;
		}

		if (component_data["FittingMode"] == "SCALE_TO_FIT")
		{
			uitext.fittingMode = GUIText::FittingMode::SCALE_TO_FIT;
		}
		else if (component_data["FittingMode"] == "KEEP_FONT_SIZE")
		{
			uitext.fittingMode = GUIText::FittingMode::KEEP_FONT_SIZE;
		}
	}

}