#include "Akkad/PlatformMacros.h"
#include "Akkad/Input/Input.h"
#include "WindowsKeyCodes.h"

#include <Windows.h>
#include "Win32Input.h"

 bool Akkad::Win32Input::GetKeyDown(unsigned int key)
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

 float Akkad::Win32Input::GetMouseX()
 {
	 POINT p;
	 GetCursorPos(&p);
	 return p.x;
 }

 float Akkad::Win32Input::GetMouseY()
 {
	 POINT p;
	 GetCursorPos(&p);
	 return p.y;
 }
