#pragma once
#include "KeyCodes.h"
namespace Akkad {

	class Input {
	public:
		virtual bool GetKeyDown(unsigned int key) = 0;
		virtual bool GetMouseDown(MouseButtons button) = 0;
		virtual float GetMouseX() = 0;
		virtual float GetMouseY() = 0;
	};
}
