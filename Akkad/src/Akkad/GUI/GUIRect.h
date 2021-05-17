#pragma once
#include "Akkad/Graphics/Rect.h"

namespace Akkad {
	namespace GUI {
		enum class ConstraintType { CENTER_CONSTRAINT,
			RELATIVE_CONSTRAINT, PIXEL_CONSTRAINT, ASPECT_CONSTRAINT };

		struct Constraint
		{
			ConstraintType type;
			float constraintValue;
		};

		class GUIRect
		{
		public:
			GUIRect();

			void SetParentPos(glm::vec2 pos) { m_ParentPos = pos; }
			void SetParentSize(glm::vec2 size) { m_ParentSize = size; }

			void SetWidthConstraint(Constraint constraint);
			void SetHeightConstraint(Constraint constraint);
			void SetXConstraint(Constraint constraint);
			void SetYConstraint(Constraint constraint);

			Constraint GetWidthConstraint() { return m_widthConstraint; }
			Constraint GetHeightConstraint() { return m_heightConstraint; }

			Constraint GetXConstraint() { return m_xConstraint; }
			Constraint GetYConstraint() { return m_yConstraint; }



			Graphics::Rect GetRect() { return m_Rect; }
		private:
			void RecalculateRect();
			void SetWidth(ConstraintType type, float width);
			void SetHeight(ConstraintType type, float height);
			void SetX(ConstraintType type, float xPos);
			void SetY(ConstraintType type, float yPos);

			Constraint m_xConstraint;
			Constraint m_yConstraint;
			Constraint m_widthConstraint;
			Constraint m_heightConstraint;

			Graphics::Rect m_Rect;
			glm::vec2 m_ParentPos;
			glm::vec2 m_ParentSize;

			friend class Scene;
		};
	}
}