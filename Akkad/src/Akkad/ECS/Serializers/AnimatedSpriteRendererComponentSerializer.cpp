#include "AnimatedSpriteRendererComponentSerializer.h"

#include "Akkad/Application/Application.h"
#include "Akkad/Asset/AssetManager.h"
#include "Akkad/ECS/Components/SpriteRendererComponent.h"

namespace Akkad {

	void AnimatedSpriteRendererComponentSerializer::Serialize(Entity entity, json& entity_data)
	{
		auto& sprite = entity.GetComponent<AnimatedSpriteRendererComponent>();
		entity_data["AnimatedSpriteRenderer"]["MaterialID"] = sprite.materialID;
		entity_data["AnimatedSpriteRenderer"]["SortingLayer"] = sprite.sprite.GetSortingLayer();
		
		for (auto it : sprite.sprite.m_Animations)
		{
			auto desc = Application::GetAssetManager()->GetAssetByName(it.first);
			
			entity_data["AnimatedSpriteRenderer"]["Animations"].push_back(desc.assetID);
		}
	}

	void AnimatedSpriteRendererComponentSerializer::Deserialize(Entity entity, json& component_data)
	{
		auto& animated_sprite = entity.AddComponent<AnimatedSpriteRendererComponent>();
		animated_sprite.materialID = component_data["MaterialID"];
		animated_sprite.sprite.SetSortingLayer(component_data["SortingLayer"]);

		for (auto it : component_data["Animations"])
		{
			animated_sprite.sprite.AddAnimation(it);
		}

		auto desc = Application::GetAssetManager()->GetDescriptorByID(animated_sprite.materialID);

		animated_sprite.sprite.SetMaterial(desc.absolutePath);

	}
}