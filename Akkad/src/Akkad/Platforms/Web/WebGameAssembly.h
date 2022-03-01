#pragma once
#include "Akkad/Scripting/LoadedGameAssembly.h"
#ifndef AK_GAME_ASSEMBLY
class SandboxLayer;
namespace Akkad {

	class WebGameAssembly : public LoadedGameAssembly
	{
	public:
		virtual bool LoadAssembly(const char* filename) override;
		virtual bool Free() override;

		virtual void Initialize(ApplicationComponents& appComponents) override;
		virtual std::vector<std::string> GetScripts() override;
		virtual ScriptableEntity* InstantiateScript(const char* scriptName) override;
	};

}
#endif


