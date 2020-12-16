#include "Akkad/ECS/Components/Components.h"
#include "Akkad/Application/Time.h"

namespace Akkad {

	class TestScript : public ScriptableEntity {
		virtual void OnStart() override
		{
			auto tag = GetComponent<TagComponent>();
			AK_INFO("Starting scriptable entity with tag : {}", tag.Tag);
		};

		virtual void OnUpdate() override
		{
			auto& transform = GetComponent<TransformComponent>();
			transform.SetPostion(transform.GetPosition());
			glm::mat4& trans = transform.GetTransformMatrix();
			trans = glm::rotate(trans, (float)Time::GetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
		};
	};
	
	RegisterScript(TestScript, "TestScript");
	
}