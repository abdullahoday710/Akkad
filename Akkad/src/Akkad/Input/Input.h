#pragma once
#include "KeyCodes.h"
namespace Akkad {

	class Input {
	public:
		// ----------- Checking keys PER frame ---------------------
		virtual bool GetKeyDown(unsigned int key) = 0;
		virtual bool GetKeyUp(unsigned int key) = 0;
		virtual bool GetMouseDown(MouseButtons button) = 0;
		virtual bool GetMouseUp(MouseButtons button) = 0;
		// -----------------------------------------------------

		virtual bool IsKeyDown(unsigned int key) = 0;
		virtual bool IsMouseDown(MouseButtons button) = 0;

		virtual int GetMouseX() = 0;
		virtual int GetMouseY() = 0;
	};
}
