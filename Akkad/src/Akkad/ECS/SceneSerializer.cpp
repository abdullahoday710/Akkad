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

			data["Scene"]["Entities"][entityID]["Tag"] = tag;
			data["Scene"]["Entities"][entityID]["Transform"]["Position"] = { position.x, position.y, position.z };
			data["Scene"]["Entities"][entityID]["Transform"]["Rotation"] = { rotation.x, rotation.y, rotation.z };

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
					auto& transform = e.GetComponent<TransformComponent>();
					transform.SetPostion(position);
					transform.SetRotation(rotation);
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

			}
		}
		file.close();
	}
}