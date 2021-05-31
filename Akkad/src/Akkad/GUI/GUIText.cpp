#include "GUIText.h"

#include <glad/glad.h>

#include <sstream>
#include <iostream>
namespace Akkad {
	namespace GUI {

		void GUIText::SetFont(std::string filepath)
		{
			m_Font = CreateSharedPtr<Font>(filepath);
		}

		void GUIText::SetFontSize(unsigned int sizePixels)
		{
			if (IsValid())
			{
				m_Font->SetFontPixelScaling(sizePixels);
			}
		}

		void GUIText::SetBoundingBox(Graphics::Rect boundingBox)
		{
			m_BoundingBox = boundingBox;
		}

		bool GUIText::IsValid()
		{
			if (m_Font != nullptr)
			{
				return true;
			}
			return false;
		}

	}
}

