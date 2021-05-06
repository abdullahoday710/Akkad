#pragma once
#include <glm/glm.hpp>

namespace Akkad {
	namespace Graphics {

		class Rect {
		public:
			Rect() {}

			Rect(glm::vec2 min, glm::vec2 max) {
				m_Min = min;
				m_Max = max;
				m_Width = max.x - min.x;
				m_Height = min.y - max.y;
			}

			Rect(glm::vec2 position, float width, float height)
			{
				m_Min = position;
				m_Max.x = position.x + width;
				m_Max.y = position.y - height;
				m_Width = width;
				m_Height = height;
			}
			glm::vec2 GetMin() { return m_Min; };
			glm::vec2 GetMax() { return m_Max; };

			void SetMin(glm::vec2 min) { m_Min = min; };
			void SetMax(glm::vec2 max) { m_Max = max; };

			glm::vec2 GetCenter()
			{
				glm::vec2 center;
				center.x = m_Min.x + (m_Width / 2);
				center.y = m_Max.y + (m_Height / 2);
				return center;
			}

		private:
			glm::vec2 m_Min;
			glm::vec2 m_Max;
			float m_Width;
			float m_Height;
		};
	}
}