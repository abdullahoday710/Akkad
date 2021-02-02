#include "GameAssembly.h"

#include "Akkad/ECS/Components/Components.h"

namespace Akkad {

	class TestScript : public ScriptableEntity {
		virtual void OnStart() override
		{
			auto& tag = GetComponent<TagComponent>();
			AK_INFO("Starting scriptable entity with tag : {}", tag.Tag);
		};

		virtual void OnUpdate() override
		{
			auto input = GameAssembly::GetInput();
			auto time = GameAssembly::GetTimeManager();

			auto& transform = GetComponent<TransformComponent>();

			if (input->GetKeyDown(AK_KEY_W))
			{
				glm::vec3& position = transform.GetPosition();

				position.y += 0.9 * time->GetDeltaTime();
			}
			if (input->GetKeyDown(AK_KEY_A))
			{
				glm::vec3& position = transform.GetPosition();

				position.x -= 0.9 * time->GetDeltaTime();
			}
			if (input->GetKeyDown(AK_KEY_S))
			{
				glm::vec3& position = transform.GetPosition();

				position.y -= 0.9 * time->GetDeltaTime();
			}
			if (input->GetKeyDown(AK_KEY_D))
			{
				glm::vec3& position = transform.GetPosition();

				position.x += 0.9 * time->GetDeltaTime();
			}

			if (input->GetKeyDown(AK_KEY_SPACE))
			{
				glm::vec3& rotation = transform.GetRotation();


				rotation.z += 0.5 * time->GetDeltaTime();
			}


		};
	};

	RegisterScript(TestScript, "TestScript");

}