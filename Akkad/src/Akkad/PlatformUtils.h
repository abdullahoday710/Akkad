#pragma once
#include <string>

class PlatformUtils {
public:
	static std::string OpenFileDialog(bool changeCurrentWorkingDirectory = false);
	static std::string OpenDirectoryDialog(std::string title = "Please select a folder");
};