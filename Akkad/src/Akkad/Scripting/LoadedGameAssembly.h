#pragma once
#include <string>
#include <vector>

namespace Akkad {
	class ScriptableEntity;
	class ApplicationComponents;

	// Types for "exported" dynamic library functions
	typedef void (*INITLIBFN)(ApplicationComponents& appComponents);
	typedef std::vector<std::string>& (*GETSCRIPTSFN)();
	typedef ScriptableEntity* (*INSTANTIATESCRIPTFN)(const char* scriptName);

	class LoadedGameAssembly
	{
	public:
		virtual void LoadAssembly(const char* filename) = 0;
		virtual bool Free() = 0;

		virtual void Initialize(ApplicationComponents& appComponents) = 0;
		virtual std::vector<std::string> GetScripts() = 0;
		virtual ScriptableEntity* InstantiateScript(const char* scriptName) = 0;

	};

}