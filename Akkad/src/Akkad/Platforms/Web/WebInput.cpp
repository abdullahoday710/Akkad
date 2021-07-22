#include "WebInput.h"

#include "WebWindow.h"
namespace Akkad {
	bool WebInput::GetKeyDown(unsigned int key)
	{
		return WebWindow::m_KeyStates[key];
	}
	bool WebInput::GetMouseDown(MouseButtons button)
	{
		return false;
	}
	int WebInput::GetMouseX()
	{
		return 0;
	}
	int WebInput::GetMouseY()
	{
		return 0;
	}
}