#include "Akkad/Akkad.h"

namespace Akkad
{
	class TestCameraControllerScript : public ScriptableEntity
	{
		virtual void OnUpdate() override
		{
			auto& transform = GetComponent<TransformComponent>();

			if (Input::GetKeyDown(AK_KEY_W))
			{
				transform.GetPosition().y += 0.5f * Time::GetDeltaTime();
			}

			if (Input::GetKeyDown(AK_KEY_A))
			{
				transform.GetPosition().x -= 0.5f * Time::GetDeltaTime();
			}

			if (Input::GetKeyDown(AK_KEY_S))
			{
				transform.GetPosition().y -= 0.5f * Time::GetDeltaTime();
			}

			if (Input::GetKeyDown(AK_KEY_D))
			{
				transform.GetPosition().x += 0.5f * Time::GetDeltaTime();
			}
		}
	};

	RegisterScript(TestCameraControllerScript, "TestCameraControllerScript");
	
}