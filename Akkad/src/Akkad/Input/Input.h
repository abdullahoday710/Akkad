#pragma once

namespace Akkad {

	class Input {
	public:
		virtual bool GetKeyDown(unsigned int key) = 0;
		virtual float GetMouseX() = 0;
		virtual float GetMouseY() = 0;
	};
}
