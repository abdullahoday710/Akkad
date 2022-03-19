#pragma once
#include "GUIRect.h"
#include "GUIText.h"
namespace Akkad {
	namespace GUI {
		class GUITextInput
		{
		public:
			void SetTextInputColor(glm::vec3 color);
			void SetTextColor(glm::vec3 color);
			void SetTextInputRect(GUIRect rect);
			void SetTextAlignment(GUIText::Alignment alignment);
			void AddCharacter(char characater);
			void RemoveCharacter();

			GUIText& GetUIText() { return m_uitext; }
			glm::vec3 GetTextInputColor() { return m_TextInputColor; }
			glm::vec3 GetTextColor() { return m_TextColor; }
			GUIRect GetTextInputRect();
			std::string GetText() { return m_TextValue; }
		private:
			glm::vec3 m_TextInputColor = { 1,1,1 };
			glm::vec3 m_TextColor = { 0,0,0 };
			GUIRect m_TextInputRect;
			GUIText m_uitext;

			std::string m_TextValue;

		};
	}
}

