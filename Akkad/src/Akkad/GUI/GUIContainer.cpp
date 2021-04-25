#include "GUIContainer.h"

#include <glm/gtc/matrix_transform.hpp>
namespace Akkad {
	namespace GUI {

		void GUIContainer::SetScreenSize(glm::vec2 size)
		{
			m_Projection = glm::ortho(0.0f, size.x, 0.0f, size.y);
			m_ScreenSize = size;
		}
	}
}

