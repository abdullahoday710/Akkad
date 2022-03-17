#pragma once
#include "GUIRect.h"

#include <glm/glm.hpp>

namespace Akkad {
	namespace GUI {
		class GUISlider {
		public:
			GUIRect GetSliderRect() { return m_SliderRect; }
			GUIRect& GetKnobRect() { return m_KnobRect; }
			glm::vec3 GetSliderColor() { return m_SliderColor; }
			glm::vec3 GetKnobColor() { return m_KnobColor; }

			void SetSliderColor(glm::vec3 color) { m_SliderColor = color; }
			void SetKnobColor(glm::vec3 color) { m_KnobColor = color; }
			void SetSliderRect(GUIRect rect);
			void SetKnobX(float x);

			float GetSliderValue() { return m_SliderValue; };

		private:
			glm::vec3 m_SliderColor;
			glm::vec3 m_KnobColor;
			float m_SliderValue = 0.0f;
			float m_KnobX = 0.0f;

			GUIRect m_SliderRect;
			GUIRect m_KnobRect;


		};
	}
}