#include "SpriteRendererComponentSerializer.h"

#include "Akkad/Application/Application.h"
#include "Akkad/Asset/AssetManager.h"
#include "Akkad/ECS/Components/SpriteRendererComponent.h"
namespace Akkad {

	void SpriteRendererComponentSerializer::Serialize(Entity entity, json& entity_data)
	{
		auto& sprite = entity.GetComponent<SpriteRendererComponent>();
		entity_data["SpriteRenderer"]["MaterialID"] = sprite.materialID;
		entity_data["SpriteRenderer"]["SortingLayer"] = sprite.sprite.GetSortingLayer();
	}

	void SpriteRendererComponentSerializer::Deserialize(Entity entity, json& component_data)
	{
		std::string materialID = component_data["MaterialID"];
		std::string sortingLayer = component_data["SortingLayer"];

		auto& spriteRenderer = entity.AddComponent<SpriteRendererComponent>();
		spriteRenderer.materialID = materialID;
		spriteRenderer.sprite.SetSortingLayer(sortingLayer);

		auto desc = Application::GetAssetManager()->GetDescriptorByID(materialID);

		spriteRenderer.sprite.SetMaterial(desc.absolutePath);
	}
}