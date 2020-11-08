#include "Akkad/PlatformMacros.h"
#include "Akkad/Input/Input.h"
#include "WindowsKeyCodes.h"

#include <Windows.h>

#ifdef AK_PLATFORM_WINDOWS

bool Input::GetKeyDown(unsigned int key) {
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

 float Input::GetMouseX(){
	 POINT p;
	 GetCursorPos(&p);
	 return p.x;
 }
 float Input::GetMouseY(){
	 POINT p;
	 GetCursorPos(&p);
	 return p.y;
 }

#endif // AK_PLATFORM_WINDOWS
