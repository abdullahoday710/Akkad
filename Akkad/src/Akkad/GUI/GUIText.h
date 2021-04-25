#pragma once
#include "Akkad/core.h"

#include "Font.h"

#include <string>
#include <vector>

namespace Akkad {

	namespace GUI {

		class GUIText
		{
		public:
			GUIText() {}
			GUIText(SharedPtr<Font> font)
			{
				m_Font = font;
			}
			GUIText(std::string text, SharedPtr<Font> font) {
				m_Font = font;
				SetText(text);
			}

			void SetFont(SharedPtr<Font> font) { m_Font = font; }
			SharedPtr<Font> GetFont() { return m_Font; }


			struct Line {
				std::string text = "";
				unsigned int size = 0; // the size of the text line (in pixels) relative to screen space
				float yOffset = 0.0f;
			};
			void SetText(std::string text);
			void SetPosition(glm::vec2 position);
			glm::vec2 GetPosition() { return m_Position; }
			unsigned int m_MaxLineSize = 1000;

			std::vector<Line> m_Lines;
			std::string m_Text;
		private:
			bool PushWord(std::string word);
			SharedPtr<Font> m_Font;
			glm::vec2 m_Position = {0, 0};
			float m_Scale = 1.0f;
		};
	}
}

