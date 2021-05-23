#include "Win32GameAssembly.h"

#include "Akkad/core.h"
#include "Akkad/Application/Application.h"
#include "Akkad/Scripting/LoadedGameAssembly.h"

#include <Windows.h>
#include <filesystem>

namespace Akkad {

	bool Win32GameAssembly::LoadAssembly(const char* filename)
	{
		std::string filenameExt = filename;
		filenameExt += ".dll";
		std::filesystem::path path = filenameExt;

		HINSTANCE hDLL = NULL;

		if (path.is_absolute())
		{
			std::string dllName = path.filename().string();
			auto dllPath = path.remove_filename();

			SetDllDirectoryA(dllPath.string().c_str());

			hDLL = LoadLibraryA(dllName.c_str());
		}

		else if (path.is_relative())
		{
			hDLL = LoadLibraryA(filenameExt.c_str());
		}

		if (hDLL != NULL)
		{
			m_Handle = (void*)hDLL;
			return true;
		}
		return false;
	}

	bool Win32GameAssembly::Free()
	{
		bool result = FreeLibrary((HMODULE)m_Handle);
		return result;
	}

	void Win32GameAssembly::Initialize(ApplicationComponents& appComponents)
	{
		AK_ASSERT(m_Handle != nullptr, "Game assembly not present !");
		INITLIBFN InitLibraryFn = (INITLIBFN)GetProcAddress((HMODULE)m_Handle, "Init");
		AK_ASSERT(InitLibraryFn != NULL, "Required function is not found !");

		InitLibraryFn(appComponents);
	
	}

	std::vector<std::string> Win32GameAssembly::GetScripts()
	{
		AK_ASSERT(m_Handle != nullptr, "Game assembly not present !!!");
		GETSCRIPTSFN getScriptsFn = (GETSCRIPTSFN)GetProcAddress((HMODULE)m_Handle, "GetScripts");
		AK_ASSERT(getScriptsFn != NULL, "Required function is not found !");

		std::vector<std::string> scripts;
		scripts = getScriptsFn();

		return scripts;
	}

	ScriptableEntity* Win32GameAssembly::InstantiateScript(const char* scriptName)
	{
		AK_ASSERT(m_Handle != nullptr, "Game assembly not present !!!");
		INSTANTIATESCRIPTFN instantiateScriptFn = (INSTANTIATESCRIPTFN)GetProcAddress((HMODULE)m_Handle, "InstantiateScript");
		AK_ASSERT(instantiateScriptFn != NULL, "Required function is not found !");

		ScriptableEntity* entity;
		entity = instantiateScriptFn(scriptName);

		return entity;

	}
}