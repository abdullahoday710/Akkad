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

				m_Position = position;
			}

			Rect(glm::vec2 min, glm::vec2 max)
			{
				m_Min = min;
				m_Max = max;
				m_Width = abs(max.x - min.x);
				m_Height = abs(max.y - min.y);

				m_Position.x = m_Min.x + (m_Width / 2);
				m_Position.y = m_Max.y + (m_Height / 2);

			}

			Rect(glm::vec2 position, float width, float height)
			{
				m_Width = width;
				m_Height = height;

				SetPosition(position);
			}
			glm::vec2 GetMin() { return m_Min; };
			glm::vec2 GetMax() { return m_Max; };

			void SetMin(glm::vec2 min) { m_Min = min; };
			void SetMax(glm::vec2 max) { m_Max = max; };

			void SetPosition(glm::vec2 position)
			{
				SetX(position.x);
				SetY(position.y);
				m_Position = position;
			}

			void SetX(float xpos)
			{
				m_Min.x = xpos - (m_Width / 2);
				m_Max.x = xpos + (m_Width / 2);

				m_Position.x = xpos;
			}

			void SetY(float ypos)
			{
				m_Min.y = ypos - (m_Height / 2);
				m_Max.y = ypos + (m_Height / 2);

				m_Position.y = ypos;
			}

			void SetWidth(float width)
			{

				m_Min.x = m_Position.x - (width / 2);
				m_Max.x = m_Position.x + (width / 2);

				m_Width = width;
			};

			void SetHeight(float height)
			{

				m_Min.y = m_Position.y - (height / 2);
				m_Max.y = m_Position.y + (height / 2);

				m_Height = height;
			}
			
			float GetWidth() { return m_Width; }
			float GetHeight() { return m_Height; }
			glm::vec2 GetPosition() { return m_Position; }

		protected:
			glm::vec2 m_Min;
			glm::vec2 m_Max;
			glm::vec2 m_Position;
			float m_Width;
			float m_Height;
		};
	}
}