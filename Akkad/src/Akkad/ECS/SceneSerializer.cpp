#include "SceneSerializer.h"
#include "Components/Components.h"

#include <json.hpp>
#include <fstream>
#include <iomanip>
#include <random>

namespace Akkad {
	using json = nlohmann::json;

	// TODO : generate random UUID in a better way.
	std::string GenerateRandomName() {
		static std::random_device dev;
		static std::mt19937 rng(dev());

		std::uniform_int_distribution<int> dist(0, 15);

		const char* v = "0123456789abcdef";
		const bool dash[] = { 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0 };

		std::string res;
		for (int i = 0; i < 16; i++) {
			if (dash[i]) res += "-";
			res += v[dist(rng)];
			res += v[dist(rng)];
		}
		return res;
	}

	void SceneSerializer::Serialize()
	{
		json data;
		
		auto view = m_Scene->m_Registry.view<TagComponent, TransformComponent>();
		std::string entityID;

		for (auto entity : view)
		{
			entityID = GenerateRandomName();

			auto& tag = view.get<TagComponent>(entity).Tag;
			auto& transform = view.get<TransformComponent>(entity);

			glm::vec3 position = transform.GetPosition();

			data["Scene"]["Entities"][entityID]["Tag"] = tag;
			data["Scene"]["Entities"][entityID]["Transform"]["Position"] = { position.x, position.y, position.z };

			Entity activeEntity(entity, m_Scene);

			if (activeEntity.HasComponent<SpriteRendererComponent>())
			{
				auto& sprite = activeEntity.GetComponent<SpriteRendererComponent>();
				data["Scene"]["Entities"][entityID]["SpriteRenderer"]["Color"] = {
					sprite.color.x,
					sprite.color.y,
					sprite.color.z
				};
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
		
		std::ofstream output("res/scenes/test.json");
		output << std::setw(4) << data << std::endl;
		
	}

	void SceneSerializer::Deserialize()
	{

	}
}