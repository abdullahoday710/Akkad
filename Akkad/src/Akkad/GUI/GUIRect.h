#pragma once
#include "Akkad/Graphics/Rect.h"

namespace Akkad {
	class Scene;
	namespace GUI {
		enum class ConstraintType {
			CENTER_CONSTRAINT,
			RELATIVE_CONSTRAINT,
			PIXEL_CONSTRAINT,
			ASPECT_CONSTRAINT,
			PREVIOUS_CHILD_CONSTRAINT,};

		enum class AnchorType
		{
			TOP_LEFT, TOP_RIGHT, BOTTOM_LEFT, BOTTOM_RIGHT
		};

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
			void SetPreviousChild(Graphics::Rect child);

			void SetWidthConstraint(Constraint constraint);
			void SetHeightConstraint(Constraint constraint);
			void SetXConstraint(Constraint constraint);
			void SetYConstraint(Constraint constraint);

			Constraint GetWidthConstraint() { return m_widthConstraint; }
			Constraint GetHeightConstraint() { return m_heightConstraint; }

			Constraint GetXConstraint() { return m_xConstraint; }
			Constraint GetYConstraint() { return m_yConstraint; }

			AnchorType GetAnchorType() { return m_AnchorType; };
			void SetAnchorType(AnchorType type) { m_AnchorType = type; };



			Graphics::Rect GetRect() { return m_Rect; }
			Graphics::Rect GetParentRect() { return m_ParentRect; }
		private:
			void RecalculateRect();
			void SetWidth(ConstraintType type, float width);
			void SetHeight(ConstraintType type, float height);
			void SetX(ConstraintType type, float xPos);
			void SetY(ConstraintType type, float yPos);

			glm::vec2 GetAnchoredPosition(bool adjustXHeight, bool adjustYHeight);

			Constraint m_xConstraint;
			Constraint m_yConstraint;
			Constraint m_widthConstraint;
			Constraint m_heightConstraint;
			AnchorType m_AnchorType = AnchorType::TOP_LEFT;

			Graphics::Rect m_Rect;
			Graphics::Rect m_ParentRect;
			Graphics::Rect m_PreviousChild;

			friend class ::Akkad::Scene;
		};
	}
}