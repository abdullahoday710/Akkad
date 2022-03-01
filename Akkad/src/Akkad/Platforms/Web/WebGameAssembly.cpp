#include "WebGameAssembly.h"
#include "Akkad/core.h"
#include "Akkad/Application/Application.h"
#include "Akkad/Scripting/ScriptableEntity.h"
#include <emscripten.h>
#include <emscripten/threading.h>
#include <vector>

#ifndef AK_GAME_ASSEMBLY
namespace Akkad {
	extern "C" void InitGameAssembly(Akkad::ApplicationComponents * appComponents);
	extern "C" std::vector<std::string>& GetScriptsGameAssembly();
	extern "C" ScriptableEntity* InstantiateScriptGameAssembly(const char* scriptName);

	bool WebGameAssembly::LoadAssembly(const char* filename)
	{
		//#ifndef AK_GAME_ASSEMBLY
		//	m_Handle = (void*)doLoadLibrary();
		//#endif
		return true;
	}

	bool WebGameAssembly::Free()
	{
		return false;
	}

	void WebGameAssembly::Initialize(ApplicationComponents& appComponents)
	{
		auto gameassemblycache = Application::GetGameAssembly();
		//initlib(&appComponents);
		InitGameAssembly(&appComponents);
		Application::GetInstance().m_LoadedGameAssembly = gameassemblycache;
	}

	std::vector<std::string> WebGameAssembly::GetScripts()
	{
		std::cout << "test scripts" << std::endl;
		std::vector<std::string> test;
		return test;
	}

	ScriptableEntity* WebGameAssembly::InstantiateScript(const char* scriptName)
	{
		return InstantiateScriptGameAssembly(scriptName);
	}

	//std::vector<std::string> WebGameAssembly::GetScripts()
	//{
	//	//GETSCRIPTSFN getscripts = (GETSCRIPTSFN)dlsym(m_Handle, "GetScriptsGameAssembly");
	//	//AK_ASSERT(getscripts, "Required function : GetScriptsGameAssembly cannot be loaded !");
	//	std::vector<std::string> test;
	//	return test;
	//	//return GetScriptsGameAssembly();
	//}

	//ScriptableEntity* WebGameAssembly::InstantiateScript(const char* scriptName)
	//{
	//	//INSTANTIATESCRIPTFN instantfunc = (INSTANTIATESCRIPTFN)dlsym(m_Handle, "InstantiateScriptGameAssembly");
	//	//AK_ASSERT(instantfunc, "Required function : InstantiateScriptGameAssembly cannot be loaded !");
	//	std::cout << "WebGameAssembly::InstantiateScript" << std::endl;
	//	return InstantiateScriptGameAssembly(scriptName);
	//}
}
#endif

