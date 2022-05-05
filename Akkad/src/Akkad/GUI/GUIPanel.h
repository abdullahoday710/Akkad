#pragma once
#include "GUIRect.h"

namespace Akkad {
	namespace GUI {

		class GUIPanel
		{
		public:
			void SetUIRect(GUIRect rect) { m_UIRect = rect; }
			void SetColor(glm::vec3 color) { m_Color = color; }

			glm::vec3 GetColor() { return m_Color; }
			GUIRect GetUIRect() { return m_UIRect; }
			void SetTransparent(bool transparent) { m_IsTransparent = transparent; };
			bool IsTransparent() { return m_IsTransparent; };
		private:
			GUIRect m_UIRect;
			glm::vec3 m_Color = { 1 ,1, 1 };
			bool m_IsTransparent = false;
		};
	}
}


