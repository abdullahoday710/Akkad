#include "WebInput.h"

#include "WebWindow.h"
namespace Akkad {
	bool WebInput::GetKeyDown(unsigned int key)
	{
		return WebWindow::m_KeyStates[key];
	}
	bool WebInput::GetMouseDown(MouseButtons button)
	{
		switch (button)
		{
		case MouseButtons::LEFT:
		{
			return WebWindow::MouseState_Left;
			break;
		}
		case MouseButtons::RIGHT:
		{
			return WebWindow::MouseState_Right;
			break;
		}
		case MouseButtons::MIDDLE:
		{
			return WebWindow::MouseState_Middle;
			break;
		}
		default:
			break;
		}
	}
	int WebInput::GetMouseX()
	{
		return WebWindow::MouseX;
	}
	int WebInput::GetMouseY()
	{
		return WebWindow::MouseY;
	}
}