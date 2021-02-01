#pragma once
#include "ScriptFactory.h"

#include <Akkad/PlatformMacros.h>

#include <string>
#include <vector>

#ifdef AK_PLATFORM_WINDOWS

#ifdef AK_GAME_ASSEMBLY
#define AK_GAME_ASSEMBLY_API __declspec(dllexport)
#else
#define AK_GAME_ASSEMBLY_API __declspec(dllimport)
#endif // AK_GAME_ASSEMBLY

#endif // AK_PLATFORM_WINDOWS

namespace Akkad {
	class ApplicationComponents;
	class ScriptableEntity;

	class GameAssembly
	{
	public:
		static GameAssembly& GetInstance()
		{
			static GameAssembly instance;
			return instance;
		}
		void Init(ApplicationComponents& appComponents);
		ApplicationComponents* GetApplicationComponents() { return m_ApplicationComponents; }
		ScriptFactory& GetFactory() { return m_Factory; }

	private:
		GameAssembly() : m_Factory() {}
		~GameAssembly() {}

		ScriptFactory m_Factory;
		ApplicationComponents* m_ApplicationComponents = nullptr;
	};
}

#define RegisterScript(type,name)\
namespace {\
	bool registerScript = Akkad::GameAssembly::GetInstance().GetFactory().RegisterScriptObject<type>(name);\
}

// ----------------- Exports ---------------------

extern "C" AK_GAME_ASSEMBLY_API void Init(Akkad::ApplicationComponents& appComponents);
extern "C" AK_GAME_ASSEMBLY_API std::vector<std::string>& GetScripts();
extern "C" AK_GAME_ASSEMBLY_API Akkad::ScriptableEntity* InstantiateScript(const char* scriptName);

// -----------------------------------------------