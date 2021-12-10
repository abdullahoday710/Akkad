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

			void Reset(glm::vec2 position, float width, float height)
			{
				m_Width = width;
				m_Height = height;

				SetPosition(position);
			}

			bool operator==(const Rect& other) {
				if (other.m_Position.x == m_Position.x)
				{
					if (other.m_Position.y == m_Position.y) {

						return true;
					}
				}
				return false;
			}

			bool operator!=(const Rect& other) {
				if (other.m_Position.x != m_Position.x)
				{
					return true;
				}

				else if (other.m_Position.y != m_Position.y)
				{
					return true;
				}
				return false;
			}
			glm::vec2 GetMin() { return m_Min; };
			glm::vec2 GetMax() { return m_Max; };
			glm::vec2 GetMinTextureCoords() { return m_MinTextureCoords; };
			glm::vec2 GetMaxTextureCoords() { return m_MaxTextureCoords; };

			void SetBounds(glm::vec2 min, glm::vec2 max)
			{
				m_Min = min;
				m_Max = max;

				m_Width = max.x - min.x;
				m_Height = max.y - min.y;

				m_Position.x = m_Min.x + (m_Width / 2);
				m_Position.y = m_Max.y + (m_Height / 2);

			}

			void SetTextureCoords(glm::vec2 min, glm::vec2 max)
			{
				m_MinTextureCoords = min;
				m_MaxTextureCoords = max;
			}

			void SetPosition(glm::vec2 position)
			{
				SetX(position.x);
				SetY(position.y);
				m_Position = position;
			}

			void SetX(float xpos)
			{
				if (xpos < 0)
				{
					m_Min.x = xpos + (m_Width / 2);
					m_Max.x = xpos - (m_Width / 2);
				}
				else
				{
					m_Min.x = xpos - (m_Width / 2);
					m_Max.x = xpos + (m_Width / 2);
				}

				m_Position.x = xpos;
			}

			void SetY(float ypos)
			{
				if (ypos < 0)
				{
					m_Min.y = ypos + (m_Height / 2);
					m_Max.y = ypos - (m_Height / 2);
				}

				else
				{
					m_Min.y = ypos - (m_Height / 2);
					m_Max.y = ypos + (m_Height / 2);
				}

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

			glm::vec2 m_MinTextureCoords = {0.0f, 1.0f};
			glm::vec2 m_MaxTextureCoords = {1.0f, 0.0f};

			float m_Width;
			float m_Height;
		};
	}
}