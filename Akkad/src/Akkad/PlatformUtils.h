#pragma once
#include <string>

namespace Akkad {

	class PlatformUtils {
	public:
		static std::string OpenFileDialog(bool changeCurrentWorkingDirectory = false);
		static std::string OpenDirectoryDialog(std::string title = "Please select a folder");
	};
}
