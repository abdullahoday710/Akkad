#include "Win32Helpers.h"
#include <Windows.h>

wchar_t* convertCharArrayToLPCWSTR(const char* charArray)
{
    
    wchar_t* wString = new wchar_t[4096];
    MultiByteToWideChar(CP_ACP, 0, charArray, -1, wString, 4096);
    return wString;
    
}

std::string convertLPCWSTRToString(const LPCWSTR lpcwstr)
{
    std::wstring wstr = lpcwstr;
    std::string strTo;
    char* szTo = new char[wstr.length() + 1];
    szTo[wstr.size()] = '\0';
    WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, szTo, (int)wstr.length(), NULL, NULL);
    strTo = szTo;
    delete[] szTo;
    return strTo;
}
