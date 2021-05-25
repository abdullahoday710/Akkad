#pragma once
#include "Akkad/GUI/GUIText.h"

namespace Akkad {
	struct GUITextComponent
	{
		GUITextComponent() : text()
		{

		}

		GUI::GUIText _textsys;
		std::string fontAssetID;
		unsigned int fontSize = 48;
		glm::vec3 textColor = { 1,1,1 };
		std::string text;
	};
}
