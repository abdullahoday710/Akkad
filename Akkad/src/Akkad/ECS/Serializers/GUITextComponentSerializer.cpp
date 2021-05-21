#include "GUITextComponentSerializer.h"

#include "Akkad/Application/Application.h"
#include "Akkad/Asset/AssetManager.h"
#include "Akkad/ECS/Components/GUITextComponent.h"
namespace Akkad {
	void GUITextComponentSerializer::Serialize(Entity entity, json& entity_data)
	{

		auto textComponent = entity.GetComponent<GUITextComponent>();
		entity_data["GUITextComponent"]["fontAssetID"] = textComponent.fontAssetID;
		entity_data["GUITextComponent"]["fontSize"] = textComponent.fontSize;
		entity_data["GUITextComponent"]["text"] = textComponent.text.GetText();
		
		switch (textComponent.text.GetAlignment())
		{
		case GUI::GUIText::Alignment::LEFT:
		{
			entity_data["GUITextComponent"]["alignment"] = "Left";
			break;
		}
		case GUI::GUIText::Alignment::CENTER:
		{
			entity_data["GUITextComponent"]["alignment"] = "Center";
			break;
		}
		default:
			break;
		}

		glm::vec3 textcolor = textComponent.textColor;
		entity_data["GUITextComponent"]["textColor"] = { textcolor.x, textcolor.y, textcolor.z };
	}

	void GUITextComponentSerializer::Deserialize(Entity entity, json& component_data)
	{
		std::string fontAssetID = component_data["fontAssetID"];
		unsigned int fontSize = component_data["fontSize"];
		std::string text = component_data["text"];

		glm::vec3 textColor;
		textColor.x = component_data["textColor"][0];
		textColor.y = component_data["textColor"][1];
		textColor.z = component_data["textColor"][2];

		auto desc = Application::GetAssetManager()->GetDescriptorByID(fontAssetID);
		auto& uitext = entity.AddComponent<GUITextComponent>(text, fontAssetID, desc.absolutePath, fontSize, textColor);

		if (component_data["alignment"] == "Left")
		{
			uitext.text.SetAlignment(GUI::GUIText::Alignment::LEFT);
		}

		if (component_data["alignment"] == "Center")
		{
			uitext.text.SetAlignment(GUI::GUIText::Alignment::CENTER);
		}

	}

}