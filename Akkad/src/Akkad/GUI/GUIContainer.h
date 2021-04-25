#pragma once

#include <glm/glm.hpp>
namespace Akkad {

	namespace GUI {

		class GUIContainer
		{
		public:
			void SetScreenSize(glm::vec2 size);
			glm::mat4 GetProjection() { return m_Projection; }
			glm::vec2 GetScreenSize() { return m_ScreenSize; }
		private:
			glm::mat4 m_Projection;
			glm::vec2 m_ScreenSize;
		};
	}
}


