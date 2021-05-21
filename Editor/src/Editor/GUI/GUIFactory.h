#pragma once

#include "Editor/EditorLayer.h"

#include <Akkad/ECS/Entity.h>
namespace Akkad {

	class GUIFactory
	{
	public:
		static Entity AddGuiText();
		static Entity AddGuiRect();
		static Entity AddGuiButton();

	private:
		static void CheckGuiContainer();

	};
}


