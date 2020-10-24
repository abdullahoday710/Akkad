#include "Win32Helpers.h"
#include <Windows.h>

wchar_t* convertCharArrayToLPCWSTR(const char* charArray)
{
    
    wchar_t* wString = new wchar_t[4096];
    MultiByteToWideChar(CP_ACP, 0, charArray, -1, wString, 4096);
    return wString;
    
}
