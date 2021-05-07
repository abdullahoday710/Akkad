#pragma once
#include <glm/glm.hpp>

namespace Akkad {
	namespace Graphics {

		class Rect {
		public:
			Rect()
			{
				glm::vec2 position = { 0,0 };
				float width = 1;
				float height = 1;

				m_Min.x = position.x - (width / 2);
				m_Min.y = position.y - (height / 2);

				m_Max.x = position.x + (width / 2);
				m_Max.y = position.y + (height / 2);

				m_Width = width;
				m_Height = height;
			}

			Rect(glm::vec2 min, glm::vec2 max)
			{
				m_Min = min;
				m_Max = max;
				m_Width = abs(max.x - min.x);
				m_Height = abs(max.y - min.y);
			}

			Rect(glm::vec2 position, float width, float height)
			{
				m_Min.x = position.x - (width / 2);
				m_Min.y = position.y - (height / 2);

				m_Max.x = position.x + (width / 2);
				m_Max.y = position.y + (height / 2);

				m_Width = width;
				m_Height = height;
			}
			glm::vec2 GetMin() { return m_Min; };
			glm::vec2 GetMax() { return m_Max; };

			void SetMin(glm::vec2 min) { m_Min = min; };
			void SetMax(glm::vec2 max) { m_Max = max; };
			
			float GetWidth() { return m_Width; }
			float GetHeight() { return m_Height; }

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