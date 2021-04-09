#include "Akkad/PlatformMacros.h"
#include "Akkad/Input/Input.h"
#include "Akkad/Application/Application.h"
#include "Win32Window.h"

#include "WindowsKeyCodes.h"

#include <Windows.h>
#include "Win32Input.h"

namespace Akkad {

	bool Win32Input::GetKeyDown(unsigned int key)
	{
		unsigned int scancode = scanCodes[key];
		if (GetAsyncKeyState(MapVirtualKeyA(scancode, MAPVK_VSC_TO_VK)))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool Win32Input::GetMouseDown(MouseButtons button)
	{
		Win32Window* window = (Win32Window*)Application::GetInstance().GetWindow();
		if (GetAsyncKeyState(MapMouseButtonsToVKButton(button)) && window->isCursorTracked)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	int Win32Input::GetMouseX()
	{
		POINT p;
		GetCursorPos(&p);
		return p.x;

	}

	int Win32Input::GetMouseY()
	{
		POINT p;
		GetCursorPos(&p);
		return p.y;
	}

	int Win32Input::MapMouseButtonsToVKButton(MouseButtons button)
	{
		switch (button)
		{
		case MouseButtons::LEFT:
			return VK_LBUTTON;
		case MouseButtons::RIGHT:
			return VK_RBUTTON;
		case MouseButtons::MIDDLE:
			return VK_MBUTTON;
		}
	}
}
