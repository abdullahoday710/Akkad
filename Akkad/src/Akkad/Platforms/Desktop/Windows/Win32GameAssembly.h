#pragma once

#include "Akkad/Scripting/LoadedGameAssembly.h"

namespace Akkad {
	class Win32GameAssembly : public LoadedGameAssembly
	{

	public:
		virtual void LoadAssembly(const char* filename) override;
		virtual bool Free() override;

		virtual void Initialize(ApplicationComponents& appComponents) override;
		virtual std::vector<std::string> GetScripts() override;
		virtual ScriptableEntity* InstantiateScript(const char* scriptName) override;

	private:
		void* m_Handle;
	};
}