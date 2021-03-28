#pragma once
#include "Akkad/Input/Input.h"

namespace Akkad {
	class Win32Input : public Input
	{
	public:
		virtual bool GetKeyDown(unsigned int key) override;
		virtual bool GetMouseDown(MouseButtons button) override;
		virtual float GetMouseX() override;
		virtual float GetMouseY() override;
	private:
		int MapMouseButtonsToVKButton(MouseButtons button);
	};
}