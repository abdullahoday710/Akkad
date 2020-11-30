#pragma once

#include "Akkad/ECS/Components/Components.h"
namespace Akkad {

	class TestScript : public ScriptableEntity {
		virtual void OnStart() override;

		virtual void OnUpdate() override;
	};

}

