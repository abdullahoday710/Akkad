#pragma once
#include "Akkad/core.h"

#include "Font.h"

#include <string>

namespace Akkad {

	namespace GUI {

		class GUIText
		{
		public:
			GUIText(std::string text) { m_Text = text; };
			GUIText(std::string text, SharedPtr<Font> font) {
				m_Text = text;
				m_Font = font;
			}

			void SetFont(SharedPtr<Font> font) { m_Font = font; }
			SharedPtr<Font> GetFont() { return m_Font; }

			std::string m_Text;

			//void Render(float x, float y, float scale, glm::vec3 color, glm::mat4 projection);
		private:
			SharedPtr<Font> m_Font;
		};
	}
}

