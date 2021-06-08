#pragma once
#include "Akkad/GUI/GUIText.h"

namespace Akkad {
	struct GUITextComponent
	{
	public:
		GUITextComponent() : uitext()
		{
		}

		std::string fontAssetID;
		std::string text;
		glm::vec3 color = { 1,1,1 };
		unsigned int fontSize = 64;
		GUI::GUIText::Alignment alignment = GUI::GUIText::Alignment::LEFT;
		GUI::GUIText::FittingMode fittingMode = GUI::GUIText::FittingMode::KEEP_FONT_SIZE;
	private:
		GUI::GUIText uitext;
		friend class Scene;

	};
}
