#pragma once
#include "Akkad/Input/Input.h"
namespace Akkad {

	class WebInput : public Input
	{
	public:
		virtual bool GetKeyDown(unsigned int key);
		virtual bool GetMouseDown(MouseButtons button);
		virtual int GetMouseX();
		virtual int GetMouseY();
	};
}


