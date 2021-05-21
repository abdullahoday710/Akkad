#pragma once
#include "GUIRect.h"

#include <glm/glm.hpp>

#include <functional>
namespace Akkad {
	namespace GUI {

		class GUIButton
		{
		public:
			void SetUIRect(GUIRect rect) { m_UIRect = rect; }
			void SetColor(glm::vec3 color) { m_Color = color; }

			glm::vec3 GetColor() { return m_Color; }
			GUIRect GetUIRect() { return m_UIRect; }
			std::function<void()> m_Callback;

		private:
			GUIRect m_UIRect;
			glm::vec3 m_Color = {1 ,1, 1};
		};

	}
}


