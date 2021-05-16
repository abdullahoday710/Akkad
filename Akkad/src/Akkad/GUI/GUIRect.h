#pragma once
#include "Akkad/Graphics/Rect.h"

namespace Akkad {
	namespace GUI {
		enum class ConstraintType { CENTER_CONSTRAINT,
			RELATIVE_CONSTRAINT, PIXEL_CONSTRAINT, ASPECT_CONSTRAINT };

		class GUIRect
		{
		public:
			void SetParentPos(glm::vec2 pos) { m_ParentPos = pos; }
			void SetParentSize(glm::vec2 size) { m_ParentSize = size; }

			void SetWidth(ConstraintType type, float width);
			void SetHeight(ConstraintType type, float height);
			void SetX(ConstraintType type, float xPos);
			void SetY(ConstraintType type, float yPos);

			Graphics::Rect GetRect() { return m_Rect; }
		private:
			void RecalculateRect();

			Graphics::Rect m_Rect;
			glm::vec2 m_ParentPos;
			glm::vec2 m_ParentSize;
		};
	}
}