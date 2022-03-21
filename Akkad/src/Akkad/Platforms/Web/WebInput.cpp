#include "WebInput.h"

#include "WebWindow.h"
namespace Akkad {
	bool WebInput::GetKeyDown(unsigned int key)
	{
		if (WebWindow::m_KeyStatesFrame[key] == 1)
		{
			return true;
		}
		return false;
	}
	bool WebInput::GetMouseDown(MouseButtons button)
	{
		int ibutton = static_cast<int>(button);
		if (WebWindow::m_MouseStatesFrame[ibutton] == 1)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool WebInput::GetMouseUp(MouseButtons button)
	{
		int ibutton = static_cast<int>(button);
		if (WebWindow::m_MouseStatesFrame[ibutton] == 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool WebInput::IsKeyDown(unsigned int key)
	{
		if (WebWindow::m_KeyStates[key] == 1)
		{
			return true;
		}
		return false;
	}

	bool WebInput::IsMouseDown(MouseButtons button)
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

	int WebInput::GetCharacterDown()
	{
		return WebWindow::m_LastPressedChar;
	}

	bool WebInput::GetKeyUp(unsigned int key)
	{
		if (!WebWindow::m_KeyStatesFrame[key])
		{
			return true;
		}
		return false;
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