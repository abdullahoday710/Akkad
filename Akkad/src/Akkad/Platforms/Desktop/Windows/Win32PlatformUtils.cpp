#include "Akkad/PlatformMacros.h"
#include "Akkad/PlatformUtils.h"

#include "Win32Helpers.h"
#include <Windows.h>
#include <shlobj_core.h>

#ifdef AK_PLATFORM_WINDOWS

std::string openWindowsFileDialog(bool changeCurrentWorkingDirectory) {
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

    if (changeCurrentWorkingDirectory)
    {
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
    }

    else
    {
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
    }

    if (GetOpenFileName(&ofn) == true)
    {
        return convertLPCWSTRToString(ofn.lpstrFile);
    }

    else
    {
        return "";
    }
        
}

std::string PlatformUtils::OpenFileDialog(bool changeCurrentWorkingDirectory)
{
   return openWindowsFileDialog(changeCurrentWorkingDirectory);
}

std::string PlatformUtils::OpenDirectoryDialog(std::string title)
{
    char szDir[MAX_PATH];
    BROWSEINFOA bInfo;
    ZeroMemory(&bInfo, sizeof(bInfo));
    bInfo.pszDisplayName = szDir; // Address of a buffer to receive the display name of the folder selected by the user
    bInfo.lpszTitle = title.c_str(); // Title of the dialog
    bInfo.ulFlags = BIF_USENEWUI;
    bInfo.lpfn = NULL;
    bInfo.lParam = 0;
    bInfo.iImage = -1;

    LPITEMIDLIST lpItem = SHBrowseForFolderA(&bInfo);
    if (lpItem != NULL)
    {
        SHGetPathFromIDListA(lpItem, szDir);
        std::string path(szDir);
        return path;
    }

    else
    {
        return "";
    }
    
}
#endif 

