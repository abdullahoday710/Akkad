#include "SceneSerializer.h"
#include "Components/Components.h"

#include "Akkad/Random.h"
#include "Akkad/Application/Application.h"
#include "Akkad/Asset/AssetManager.h"

#include <json.hpp>
#include <fstream>
#include <iomanip>

namespace Akkad {
	using json = nlohmann::json;

	void SceneSerializer::Serialize(SharedPtr<Scene> scene, std::string& outputPath)
	{
		json data;
		
		auto view = scene->m_Registry.view<TagComponent, TransformComponent>();
		std::string entityID;
		data["Scene"]["Name"] = scene->m_Name;
		for (auto entity : view)
		{
			entityID = Random::GenerateRandomUUID();

			auto& tag = view.get<TagComponent>(entity).Tag;
			auto& transform = view.get<TransformComponent>(entity);

			glm::vec3 position = transform.GetPosition();
			glm::vec3 rotation = transform.GetRotation();
			glm::vec3 scale = transform.GetScale();

			data["Scene"]["Entities"][entityID]["Tag"] = tag;
			data["Scene"]["Entities"][entityID]["Transform"]["Position"] = { position.x, position.y, position.z };
			data["Scene"]["Entities"][entityID]["Transform"]["Rotation"] = { rotation.x, rotation.y, rotation.z };
			data["Scene"]["Entities"][entityID]["Transform"]["Scale"] = { scale.x, scale.y, scale.z };

			Entity activeEntity = scene->GetEntity(entity);

			if (activeEntity.HasComponent<SpriteRendererComponent>())
			{
				auto& sprite = activeEntity.GetComponent<SpriteRendererComponent>();
				data["Scene"]["Entities"][entityID]["SpriteRenderer"]["MaterialID"] = sprite.materialID;
			}

			if (activeEntity.HasComponent<ScriptComponent>())
			{
				auto& script = activeEntity.GetComponent<ScriptComponent>();
				data["Scene"]["Entities"][entityID]["Script"]["Name"] = script.ScriptName;
			}

			if (activeEntity.HasComponent<CameraComponent>())
			{
				auto& cameraComponent = activeEntity.GetComponent<CameraComponent>();
				auto& camera = cameraComponent.camera;

				switch (camera.GetProjectionType())
				{
				case CameraProjection::Orthographic:
					data["Scene"]["Entities"][entityID]["CameraComponent"]["Camera"]["Projection"] = "Orthographic";
					break;
				case CameraProjection::Perspective:
					data["Scene"]["Entities"][entityID]["CameraComponent"]["Camera"]["Projection"] = "Perspective";
					break;
				}

			}

			if (activeEntity.HasComponent<RigidBody2dComponent>())
			{
				auto& body2dcomponent = activeEntity.GetComponent<RigidBody2dComponent>();
				data["Scene"]["Entities"][entityID]["RigidBody2D"]["Friction"] = body2dcomponent.friction;
				data["Scene"]["Entities"][entityID]["RigidBody2D"]["Density"] = body2dcomponent.density;

				switch (body2dcomponent.shape)
				{
				case BodyShape::POLYGON_SHAPE:
				{
					data["Scene"]["Entities"][entityID]["RigidBody2D"]["Shape"] = "Polygon";
					break;
				}

				}

				switch (body2dcomponent.type)
				{

				case BodyType::STATIC:
				{
					data["Scene"]["Entities"][entityID]["RigidBody2D"]["Type"] = "Static";
					break;
				}

				case BodyType::DYNAMIC:
				{
					data["Scene"]["Entities"][entityID]["RigidBody2D"]["Type"] = "Dynamic";
					break;
				}

				}
			}
		}

		std::ofstream output;
		output.open(outputPath, std::ios::trunc);
		output << std::setw(4) << data << std::endl;
		output.close();
		
	}

	void SceneSerializer::Deserialize(SharedPtr<Scene> scene, std::string filepath)
	{
		std::ifstream file;
		file.open(filepath);
		json data;
		file >> data;

		std::string sceneName = data["Scene"]["Name"];
		scene->m_Name = sceneName;

		for (auto& entity : data["Scene"]["Entities"].items()) {
			Entity e = scene->AddEntity();

			for (auto& component : data["Scene"]["Entities"][entity.key()].items())
			{
				auto componentData = component.value();
				if (component.key() == "Transform")
				{
					glm::vec3 position({componentData["Position"][0],componentData["Position"][1],componentData["Position"][2]});
					glm::vec3 rotation({ componentData["Rotation"][0],componentData["Rotation"][1],componentData["Rotation"][2] });
					glm::vec3 scale({ componentData["Scale"][0],componentData["Scale"][1],componentData["Scale"][2] });
					auto& transform = e.GetComponent<TransformComponent>();
					transform.SetPostion(position);
					transform.SetRotation(rotation);
					transform.SetScale(scale);
					continue;
				}

				else if (component.key() == "Tag")
				{
					auto& tag = e.GetComponent<TagComponent>();
					std::string tagstr = componentData;
					tag.Tag = tagstr;
					continue;
				}

				else if (component.key() == "SpriteRenderer")
				{
					std::string materialID = componentData["MaterialID"];
					auto& spriteRenderer = e.AddComponent<SpriteRendererComponent>();
					spriteRenderer.materialID = materialID;

					auto desc = Application::GetAssetManager()->GetDescriptorByID(materialID);

					spriteRenderer.material = Graphics::Material::LoadFile(desc.absolutePath);

					continue;
				}

				else if (component.key() == "Script")
				{
					auto& script = e.AddComponent<ScriptComponent>(componentData["Name"]);
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

					e.AddComponent<CameraComponent>(projtype);
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

					e.AddComponent<RigidBody2dComponent>(type, shape, density, friction);
					continue;
				}

			}
		}
		file.close();
	}
}