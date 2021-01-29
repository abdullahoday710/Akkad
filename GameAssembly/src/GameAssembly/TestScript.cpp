#include "GameAssembly.h"

#include "Akkad/ECS/Components/Components.h"
#include "Akkad/Application/Time.h"

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

			rotation.z = 0.5 + rotation.z;

		};
	};

	RegisterScript(TestScript, "TestScript");

}