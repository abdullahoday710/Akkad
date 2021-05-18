#include "SceneSerializer.h"
#include "Components/Components.h"

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
			auto& tag = entity.GetComponent<TagComponent>().Tag;
			entity_data["Tag"] = tag;
		}

		if (entity.HasComponent<TransformComponent>())
		{
			auto& transform = entity.GetComponent<TransformComponent>();
			glm::vec3 position = transform.GetPosition();
			glm::vec3 rotation = transform.GetRotation();
			glm::vec3 scale = transform.GetScale();

			entity_data["Transform"]["Position"] = { position.x, position.y, position.z };
			entity_data["Transform"]["Rotation"] = { rotation.x, rotation.y, rotation.z };
			entity_data["Transform"]["Scale"] = { scale.x, scale.y, scale.z };
		}

		if (entity.HasComponent<SpriteRendererComponent>())
		{
			auto& sprite = entity.GetComponent<SpriteRendererComponent>();
			entity_data["SpriteRenderer"]["MaterialID"] = sprite.materialID;
			entity_data["SpriteRenderer"]["SortingLayer"] = sprite.sortingLayer;
		}

		if (entity.HasComponent<ScriptComponent>())
		{
			auto& script = entity.GetComponent<ScriptComponent>();
			entity_data["Script"]["Name"] = script.ScriptName;
		}

		if (entity.HasComponent<CameraComponent>())
		{
			auto& cameraComponent = entity.GetComponent<CameraComponent>();
			auto& camera = cameraComponent.camera;

			switch (camera.GetProjectionType())
			{
			case CameraProjection::Orthographic:
				entity_data["CameraComponent"]["Camera"]["Projection"] = "Orthographic";
				break;
			case CameraProjection::Perspective:
				entity_data["CameraComponent"]["Camera"]["Projection"] = "Perspective";
				break;
			}

		}

		if (entity.HasComponent<RigidBody2dComponent>())
		{
			auto& body2dcomponent = entity.GetComponent<RigidBody2dComponent>();
			entity_data["RigidBody2D"]["Friction"] = body2dcomponent.friction;
			entity_data["RigidBody2D"]["Density"] = body2dcomponent.density;

			switch (body2dcomponent.shape)
			{
			case BodyShape::POLYGON_SHAPE:
			{
				entity_data["RigidBody2D"]["Shape"] = "Polygon";
				break;
			}

			}

			switch (body2dcomponent.type)
			{

			case BodyType::STATIC:
			{
				entity_data["RigidBody2D"]["Type"] = "Static";
				break;
			}

			case BodyType::DYNAMIC:
			{
				entity_data["RigidBody2D"]["Type"] = "Dynamic";
				break;
			}

			}
		}

		if (entity.HasComponent<GUIContainerComponent>())
		{
			entity_data["GUIContainerComponent"] = true;
		}

		if (entity.HasComponent<GUITextComponent>())
		{
			auto textComponent = entity.GetComponent<GUITextComponent>();
			entity_data["GUITextComponent"]["fontAssetID"] = textComponent.fontAssetID;
			entity_data["GUITextComponent"]["fontSize"] = textComponent.fontSize;
			entity_data["GUITextComponent"]["text"] = textComponent.text.GetText();

			glm::vec3 textcolor = textComponent.textColor;
			entity_data["GUITextComponent"]["textColor"] = { textcolor.x, textcolor.y, textcolor.z };
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
				glm::vec3 position({ componentData["Position"][0],componentData["Position"][1],componentData["Position"][2] });
				glm::vec3 rotation({ componentData["Rotation"][0],componentData["Rotation"][1],componentData["Rotation"][2] });
				glm::vec3 scale({ componentData["Scale"][0],componentData["Scale"][1],componentData["Scale"][2] });
				auto& transform = entity.GetComponent<TransformComponent>();
				transform.SetPostion(position);
				transform.SetRotation(rotation);
				transform.SetScale(scale);
				continue;
			}

			else if (component.key() == "Tag")
			{
				auto& tag = entity.GetComponent<TagComponent>();
				std::string tagstr = componentData;
				tag.Tag = tagstr;
				continue;
			}

			else if (component.key() == "SpriteRenderer")
			{
				std::string materialID = componentData["MaterialID"];
				std::string sortingLayer = componentData["SortingLayer"];

				auto& spriteRenderer = entity.AddComponent<SpriteRendererComponent>();
				spriteRenderer.materialID = materialID;
				spriteRenderer.sortingLayer = sortingLayer;

				auto desc = Application::GetAssetManager()->GetDescriptorByID(materialID);

				spriteRenderer.material = Graphics::Material::LoadFile(desc.absolutePath);

				continue;
			}

			else if (component.key() == "Script")
			{
				auto& script = entity.AddComponent<ScriptComponent>(componentData["Name"]);
				continue;
			}

			else if (component.key() == "CameraComponent")
			{
				CameraProjection projtype;
				if (componentData["Camera"]["Projection"] == "Orthographic")
				{
					projtype = CameraProjection::Orthographic;
				}

				else if (componentData["Camera"]["Projection"] == "Perspective")
				{
					projtype = CameraProjection::Perspective;
				}

				entity.AddComponent<CameraComponent>(projtype);
				continue;
			}

			else if (component.key() == "RigidBody2D")
			{
				BodyType type;
				BodyShape shape;
				float density = componentData["Density"];
				float friction = componentData["Friction"];

				if (componentData["Type"] == "Static")
				{
					type = BodyType::STATIC;
				}

				else if (componentData["Type"] == "Dynamic")
				{
					type = BodyType::DYNAMIC;
				}

				if (componentData["Shape"] == "Polygon")
				{
					shape = BodyShape::POLYGON_SHAPE;
				}

				entity.AddComponent<RigidBody2dComponent>(type, shape, density, friction);
				continue;
			}

			else if (component.key() == "GUIContainerComponent")
			{
				entity.AddComponent<GUIContainerComponent>();
			}

			else if (component.key() == "GUITextComponent")
			{
				std::string fontAssetID = componentData["fontAssetID"];
				unsigned int fontSize = componentData["fontSize"];
				std::string text = componentData["text"];
				glm::vec3 textColor;
				textColor.x = componentData["textColor"][0];
				textColor.y = componentData["textColor"][1];
				textColor.z = componentData["textColor"][2];
				auto desc = Application::GetAssetManager()->GetDescriptorByID(fontAssetID);
				entity.AddComponent<GUITextComponent>(text, fontAssetID, desc.absolutePath, fontSize, textColor);
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