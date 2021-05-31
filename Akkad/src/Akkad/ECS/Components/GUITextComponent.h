#pragma once
#include "Akkad/GUI/GUIText.h"

namespace Akkad {
	struct GUITextComponent
	{
		GUITextComponent() : uitext()
		{
		}

		GUI::GUIText uitext;
		std::string fontAssetID;
	};
}
