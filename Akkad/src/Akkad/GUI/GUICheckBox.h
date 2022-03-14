#pragma once
#include "GUIRect.h"

namespace Akkad {
	namespace GUI {

		class GUICheckBox
		{
		public:
			void SetBoxUIRect(GUIRect rect);
			void SetBoxColor(glm::vec3 color) { m_BoxColor = color; }
			void SetMarkColor(glm::vec3 color) { m_MarkColor = color; }

			bool IsChecked() { return m_IsChecked; };
			void SetCheckStatus(bool check) { m_IsChecked = check; }

			glm::vec3 GetBoxColor() { return m_BoxColor; }
			glm::vec3 GetMarkColor() { return m_MarkColor; }
			GUIRect GetBoxUIRect() { return m_BoxUIRect; }
			GUIRect GetMarkUIRect() { return m_MarkUIRect; }
		private:
			GUIRect m_BoxUIRect;
			GUIRect m_MarkUIRect;

			glm::vec3 m_BoxColor = { 1 ,1, 1 };
			glm::vec3 m_MarkColor = { 0,0,0 };
			bool m_IsChecked = false;
		};
	}
}