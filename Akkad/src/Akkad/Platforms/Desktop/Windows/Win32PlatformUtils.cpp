#include "Akkad/PlatformMacros.h"
#include "Akkad/PlatformUtils.h"

#include "Win32Helpers.h"
#include <Windows.h>

#ifdef AK_PLATFORM_WINDOWS

std::string openWindowsFileDialog() {
    OPENFILENAME ofn;
    char szFile[260];

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;

    ofn.lpstrFile = convertCharArrayToLPCWSTR(szFile);

    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = L"All\0*.*\0Akkad scene\0*.AKSCENE\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (GetOpenFileName(&ofn) == true)
    {
        return convertLPCWSTRToString(ofn.lpstrFile);
    }
    else
    {
        return "";
    }
        
}

std::string PlatformUtils::OpenFileDialog()
{
    return openWindowsFileDialog();
}
#endif 

