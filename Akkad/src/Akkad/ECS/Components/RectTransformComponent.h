#pragma once
#include "Akkad/GUI/GUIRect.h"

namespace Akkad {
	
	struct RectTransformComponent
	{
		GUI::GUIRect rect;

		Graphics::Rect GetRect() { return rect.GetRect(); }
	};
}