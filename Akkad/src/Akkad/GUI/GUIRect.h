#pragma once
#include "Akkad/Graphics/Rect.h"

namespace Akkad {
	class Scene;
	namespace GUI {
		enum class ConstraintType { CENTER_CONSTRAINT,
			RELATIVE_CONSTRAINT, PIXEL_CONSTRAINT, ASPECT_CONSTRAINT, SNAP_LEFT_CONSTRAINT };

		struct Constraint
		{
			ConstraintType type;
			float constraintValue;
		};

		class GUIRect
		{
		public:
			GUIRect();

			void SetParent(Graphics::Rect parent);

			void SetWidthConstraint(Constraint constraint);
			void SetHeightConstraint(Constraint constraint);
			void SetXConstraint(Constraint constraint);
			void SetYConstraint(Constraint constraint);

			Constraint GetWidthConstraint() { return m_widthConstraint; }
			Constraint GetHeightConstraint() { return m_heightConstraint; }

			Constraint GetXConstraint() { return m_xConstraint; }
			Constraint GetYConstraint() { return m_yConstraint; }



			Graphics::Rect GetRect() { return m_Rect; }
			Graphics::Rect GetParentRect() { return m_ParentRect; }
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
			Graphics::Rect m_ParentRect;

			friend class ::Akkad::Scene;
		};
	}
}