#include "Win32GameAssembly.h"

#include "Akkad/core.h"
#include "Akkad/Scripting/LoadedGameAssembly.h"

#include <Windows.h>

namespace Akkad {

	void Win32GameAssembly::LoadAssembly(const char* filename)
	{
		std::string filenameExt = filename;
		filenameExt = filenameExt + ".dll";

		HINSTANCE hDLL = NULL;
		hDLL = LoadLibraryA(filenameExt.c_str());
		AK_ASSERT(hDLL != NULL, "Failed to load the game assembly !!");
		m_Handle = (void*)hDLL;
	}

	bool Win32GameAssembly::Free()
	{
		bool result = FreeLibrary((HMODULE)m_Handle);
		return result;
	}

	void Win32GameAssembly::Initialize(Application& app)
	{
		AK_ASSERT(m_Handle != nullptr, "Game assembly not present !");
		INITLIBFN InitLibraryFn = (INITLIBFN)GetProcAddress((HMODULE)m_Handle, "Init");
		AK_ASSERT(InitLibraryFn != NULL, "Required function is not found !");

		InitLibraryFn(app);
	
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