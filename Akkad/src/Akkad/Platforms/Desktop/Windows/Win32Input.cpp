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
		Win32Window* window = (Win32Window*)Application::GetInstance().GetWindow();
		if (window->m_KeyStatesFrame[key] == 0)
		{
			return true;
		}

		return false;

	}

	bool Win32Input::GetKeyUp(unsigned int key)
	{
		Win32Window* window = (Win32Window*)Application::GetInstance().GetWindow();
		if (window->m_KeyStatesFrame[key] == 1)
		{
			return true;
		}

		return false;
	}

	bool Win32Input::GetMouseDown(MouseButtons button)
	{
		Win32Window* window = (Win32Window*)Application::GetInstance().GetWindow();
		
		int ibutton = static_cast<int>(button);
		if (window->m_MouseStatesFrame[ibutton] == 0)
		{
			return true;
		}
		return false;
	}

	bool Win32Input::GetMouseUp(MouseButtons button)
	{
		Win32Window* window = (Win32Window*)Application::GetInstance().GetWindow();
		int ibutton = static_cast<int>(button);
		if (window->m_MouseStatesFrame[ibutton] == 1)
		{
			return true;
		}
		return false;
	}

	int Win32Input::GetCharacterDown()
	{
		Win32Window* window = (Win32Window*)Application::GetInstance().GetWindow();
		return window->m_LastPressedCharacter;
	}

	bool Win32Input::IsKeyDown(unsigned int key)
	{
		unsigned int scancode = scanCodes[key];
		Win32Window* window = (Win32Window*)Application::GetInstance().GetWindow();
		if (GetAsyncKeyState(MapVirtualKeyA(scancode, MAPVK_VSC_TO_VK)) && window->m_HasFocus)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool Win32Input::IsMouseDown(MouseButtons button)
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
