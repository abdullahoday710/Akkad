#include "TransformComponentSerializer.h"

#include "Akkad/ECS/Components/TransformComponent.h"
namespace Akkad {

	void TransformComponentSerializer::Serialize(Entity entity, json& entity_data)
	{
		auto& transform = entity.GetComponent<TransformComponent>();
		glm::vec3 position = transform.GetPosition();
		glm::vec3 rotation = transform.GetRotation();
		glm::vec3 scale = transform.GetScale();

		entity_data["Transform"]["Position"] = { position.x, position.y, position.z };
		entity_data["Transform"]["Rotation"] = { rotation.x, rotation.y, rotation.z };
		entity_data["Transform"]["Scale"] = { scale.x, scale.y, scale.z };
	}

	void TransformComponentSerializer::Deserialize(Entity entity, json& component_data)
	{
		glm::vec3 position({ component_data["Position"][0],component_data["Position"][1],component_data["Position"][2] });
		glm::vec3 rotation({ component_data["Rotation"][0],component_data["Rotation"][1],component_data["Rotation"][2] });
		glm::vec3 scale({ component_data["Scale"][0],component_data["Scale"][1],component_data["Scale"][2] });

		auto& transform = entity.GetComponent<TransformComponent>();
		transform.SetPostion(position);
		transform.SetRotation(rotation);
		transform.SetScale(scale);
	}

}