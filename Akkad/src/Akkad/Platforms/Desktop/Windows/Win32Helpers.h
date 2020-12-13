#pragma once
#include <string>
#include <Windows.h>
wchar_t* convertCharArrayToLPCWSTR(const char* charArray);
std::string convertLPCWSTRToString(const LPCWSTR lpcwstr);
