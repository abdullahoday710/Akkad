#include "GameAssemblyHandler.h"

#include <Akkad/Logging.h>
#include <Akkad/PlatformMacros.h>
#include <Akkad/Application/Application.h>
#include <Akkad/Scripting/LoadedGameAssembly.h>

namespace Akkad {
	bool GameAssemblyHandler::s_LoadedGameAssembly = false;

	void GameAssemblyHandler::CompileGameAssembly(std::string path)
	{
		#ifdef AK_PLATFORM_WINDOWS
		path += "GameAssembly.sln";
		std::string command = "msbuild " + path;
		system(command.c_str());

		#endif // AK_PLATFORM_WINDOWS

	}

	void GameAssemblyHandler::LoadGameAssembly(std::string filename)
	{
		if (!s_LoadedGameAssembly)
		{
			auto assembly = Application::GetInstance().m_LoadedGameAssembly;
			if (assembly->LoadAssembly(filename.c_str()))
			{
				assembly->Initialize(Application::GetInstance().m_ApplicationComponents);
				s_LoadedGameAssembly = true;
			}

			else
			{
				AK_WARNING("Failed to load the game assembly, maybe the assembly was never compiled or it was deleted.");
			}

		}

	}

	void GameAssemblyHandler::FreeGameAssembly()
	{
		if (s_LoadedGameAssembly)
		{
			auto assembly = Application::GetInstance().m_LoadedGameAssembly;
			bool result = assembly->Free();
			if (result)
			{
				s_LoadedGameAssembly = false;
			}
			
		}
	}

}