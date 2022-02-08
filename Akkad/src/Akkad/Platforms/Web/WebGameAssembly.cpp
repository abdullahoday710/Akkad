#include "WebGameAssembly.h"
#include "Akkad/core.h"

#include <emscripten.h>
#include <dlfcn.h>

#ifndef AK_GAME_ASSEMBLY
EM_JS(int, doLoadLibrary, (), {
	Asyncify.handleAsync(async() => {
	try {
		var handle = await loadDynamicLibrary('GameAssembly.js', { loadAsync: true, global : true, nodelete : true, fs : FS });
		console.log(handle);
		return handle;
	}
	catch (error) {
		console.log(error);
	}
});
	});
#endif

namespace Akkad {

	bool WebGameAssembly::LoadAssembly(const char* filename)
	{
		#ifndef AK_GAME_ASSEMBLY
			m_Handle = (void*)doLoadLibrary();
		#endif
		return true;
	}

	bool WebGameAssembly::Free()
	{
		return false;
	}

	void WebGameAssembly::Initialize(ApplicationComponents& appComponents)
	{
		INITLIBFN initfunc = (INITLIBFN)dlsym(m_Handle, "InitGameAssembly");
		AK_ASSERT(initfunc, "Required function : InitGameAssembly cannot be loaded !");
		initfunc(&appComponents);
	}

	std::vector<std::string> WebGameAssembly::GetScripts()
	{
		GETSCRIPTSFN getscripts = (GETSCRIPTSFN)dlsym(m_Handle, "GetScriptsGameAssembly");
		AK_ASSERT(getscripts, "Required function : GetScriptsGameAssembly cannot be loaded !");
		return getscripts();
	}

	ScriptableEntity* WebGameAssembly::InstantiateScript(const char* scriptName)
	{
		INSTANTIATESCRIPTFN instantfunc = (INSTANTIATESCRIPTFN)dlsym(m_Handle, "InstantiateScriptGameAssembly");
		AK_ASSERT(instantfunc, "Required function : InstantiateScriptGameAssembly cannot be loaded !");
		return instantfunc(scriptName);
	}
}