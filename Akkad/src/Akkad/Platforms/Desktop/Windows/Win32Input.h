#pragma once
#include "Akkad/Input/Input.h"

namespace Akkad {
	class Win32Input : public Input
	{
	public:
		virtual bool GetKeyDown(unsigned int key) override;
		virtual bool GetMouseDown(MouseButtons button) override;
		virtual bool GetMouseUp(MouseButtons button) override;
		virtual int GetMouseX() override;
		virtual int GetMouseY() override;
	private:
		int MapMouseButtonsToVKButton(MouseButtons button);
	};
}