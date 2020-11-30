#include "TestScript.h"
#include "Akkad/Application/Time.h"

namespace Akkad {
	void TestScript::OnStart()
	{
		auto tag = GetComponent<TagComponent>();
		AK_INFO("Starting scriptable entity with tag : {}", tag.Tag);
	}

	void TestScript::OnUpdate()
	{
		auto& transform = GetComponent<TransformComponent>();

		glm::mat4& trans = transform.GetTransformMatrix();
		trans = glm::rotate(trans, (float)Time::GetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
	}

	bool registerScript = ScriptFactory::GetInstance().RegisterScriptObject<TestScript>("TestScript");
}