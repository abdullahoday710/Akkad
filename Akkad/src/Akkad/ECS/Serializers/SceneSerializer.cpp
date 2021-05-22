#include "SceneSerializer.h"
#include "ComponentSerializers.h"

#include "Akkad/ECS/Components/Components.h"
#include "Akkad/Random.h"
#include "Akkad/Application/Application.h"
#include "Akkad/Asset/AssetManager.h"

#include <json.hpp>
#include <fstream>
#include <iomanip>

namespace Akkad {
	using json = nlohmann::ordered_json;

	void SerializeEntity(Entity entity, std::string parent_id, json& data)
	{
		std::string entityID = Random::GenerateRandomUUID();
		json& entity_data = data["Scene"]["Entities"][entityID];
		if (!parent_id.empty())
		{
			entity_data["ParentID"] = parent_id;
			json child;
			child["ID"] = entityID;
			data["Scene"]["Entities"][parent_id]["children"] += child;
		}

		if (entity.HasComponent<TagComponent>())
		{
			TagComponentSerializer::Serialize(entity, entity_data);
		}

		if (entity.HasComponent<TransformComponent>())
		{
			TransformComponentSerializer::Serialize(entity, entity_data);
		}

		if (entity.HasComponent<SpriteRendererComponent>())
		{
			SpriteRendererComponentSerializer::Serialize(entity, entity_data);
		}

		if (entity.HasComponent<ScriptComponent>())
		{
			ScriptComponentSerializer::Serialize(entity, entity_data);
		}

		if (entity.HasComponent<CameraComponent>())
		{
			CameraComponentSerializer::Serialize(entity, entity_data);
		}

		if (entity.HasComponent<RigidBody2dComponent>())
		{
			RigidBody2dComponentSerializer::Serialize(entity, entity_data);
		}

		if (entity.HasComponent<GUIContainerComponent>())
		{
			entity_data["GUIContainerComponent"] = true;
		}

		if (entity.HasComponent<GUITextComponent>())
		{
			GUITextComponentSerializer::Serialize(entity, entity_data);
		}

		if (entity.HasComponent<RectTransformComponent>())
		{
			RectTransformComponentSerializer::Serialize(entity, entity_data);
		}

		if (entity.HasComponent<GUIButtonComponent>())
		{
			GUIButtonComponentSerializer::Serialize(entity, entity_data);
		}

		auto& entity_relation = entity.GetComponent<RelationShipComponent>();
		Entity current_child = entity_relation.first_child;

		for (size_t i = 0; i < entity_relation.children; i++)
		{
			auto& current_child_relation = current_child.GetComponent<RelationShipComponent>();
			if (current_child.IsValid())
			{
				SerializeEntity(current_child, entityID, data);
				current_child = current_child_relation.next;
			}
		}
	
	}

	void SceneSerializer::Serialize(SharedPtr<Scene> scene, std::string& outputPath)
	{
		json data;
		
		auto view = scene->m_Registry.view<RelationShipComponent>();
		std::string entityID;
		data["Scene"]["Name"] = scene->m_Name;
		for (auto entity : view)
		{
			Entity e = scene->GetEntity(entity);
			auto& entity_relation = e.GetComponent<RelationShipComponent>();
			if (!entity_relation.parent.IsValid())
			{
				SerializeEntity(e, "", data);
			}
		}

		std::ofstream output;
		output.open(outputPath, std::ios::trunc);
		output << std::setw(4) << data << std::endl;
		output.close();
		
	}

	void DeserializeEntity(Entity entity, std::string entity_key, SharedPtr<Scene> scene, json& data)
	{
		auto& entity_relation = entity.GetComponent<RelationShipComponent>();

		for (auto& component : data["Scene"]["Entities"][entity_key].items())
		{
			auto& componentData = component.value();

			if (component.key() == "Transform")
			{
				TransformComponentSerializer::Deserialize(entity, componentData);
				continue;
			}

			else if (component.key() == "Tag")
			{
				TagComponentSerializer::Deserialize(entity, componentData);
				continue;
			}

			else if (component.key() == "SpriteRenderer")
			{
				SpriteRendererComponentSerializer::Deserialize(entity, componentData);
				continue;
			}

			else if (component.key() == "Script")
			{
				ScriptComponentSerializer::Deserialize(entity, componentData);
				continue;
			}

			else if (component.key() == "CameraComponent")
			{
				CameraComponentSerializer::Deserialize(entity, componentData);
				continue;
			}

			else if (component.key() == "RigidBody2D")
			{
				RigidBody2dComponentSerializer::Deserialize(entity, componentData);
				continue;
			}

			else if (component.key() == "GUIContainerComponent")
			{
				entity.AddComponent<GUIContainerComponent>();
			}

			else if (component.key() == "GUITextComponent")
			{
				GUITextComponentSerializer::Deserialize(entity, componentData);
				continue;

			}

			else if (component.key() == "RectTransformComponent")
			{
				RectTransformComponentSerializer::Deserialize(entity, componentData);
				continue;

			}

			else if (component.key() == "GUIButtonComponent")
			{
				GUIButtonComponentSerializer::Deserialize(entity, componentData);
				continue;

			}

			else if (component.key() == "children")
			{
				for (auto& child : data["Scene"]["Entities"][entity_key]["children"].items())
				{
					std::string child_id = child.value()["ID"];
					Entity child_entity = scene->AddEntity();

					scene->AssignEntityToParent(entity, child_entity);

					DeserializeEntity(child_entity, child_id, scene, data);
				}
				continue;
			}



		}
	}

	void SceneSerializer::Deserialize(SharedPtr<Scene> scene, std::string filepath)
	{
		std::ifstream file;
		file.open(filepath);
		json data;
		file >> data;

		std::string sceneName = data["Scene"]["Name"];
		scene->m_Name = sceneName;

		for (auto& [key, value] : data["Scene"]["Entities"].items()) {
			if (value["ParentID"].is_null())
			{
				Entity e = scene->AddEntity();
				DeserializeEntity(e, key, scene, data);
			}
		}
		file.close();
	}
}