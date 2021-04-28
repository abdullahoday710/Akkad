#pragma once
#include "Akkad/GUI/GUIText.h"

namespace Akkad {
	struct GUITextComponent
	{
		GUITextComponent() {}
		GUITextComponent(std::string inText, std::string inFontAssetID, std::string fontAbsPath, unsigned int inFontSize, glm::vec3 inTextColor) : text()
		{
			text.SetFont(fontAbsPath, fontSize);
			text.SetText(inText);
			fontAssetID = inFontAssetID;
			fontSize = inFontSize;
			textColor = inTextColor;
		}
		GUI::GUIText text;
		std::string fontAssetID;
		unsigned int fontSize = 48;
		glm::vec3 textColor = { 1,1,1 };
	};
}
