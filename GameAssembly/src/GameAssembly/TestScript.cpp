#include "GameAssembly.h"

#include <Akkad/Application/Application.h>
#include <Akkad/Application/TimeManager.h>

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
			auto& transform = GetComponent<TransformComponent>();
			glm::vec3& rotation = transform.GetRotation();
			auto time = GameAssembly::GetInstance().GetApplicationComponents()->m_TimeManager;

			rotation.z += 0.5 * time->GetDeltaTime();

		};
	};

	RegisterScript(TestScript, "TestScript");

}