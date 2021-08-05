#pragma once
#include "ScriptFactory.h"

#include <Akkad/PlatformMacros.h>
#include <Akkad/Application/Application.h>
#include <Akkad/Application/TimeManager.h>
#include <Akkad/Input/Input.h>
#include <Akkad/Input/KeyCodes.h>
#include <Akkad/Scripting/LoadedGameAssembly.h>

#include <string>
#include <vector>

#ifdef AK_PLATFORM_WINDOWS

#ifdef AK_GAME_ASSEMBLY
#define AK_GAME_ASSEMBLY_API __declspec(dllexport)
#else
#define AK_GAME_ASSEMBLY_API __declspec(dllimport)
#endif // AK_GAME_ASSEMBLY

#elif defined(__EMSCRIPTEN__)
#include <emscripten.h>
#define AK_GAME_ASSEMBLY_API EMSCRIPTEN_KEEPALIVE
#endif // __EMSCRIPTEN__


namespace Akkad {

	class GameAssembly
	{
	public:
		static GameAssembly& GetInstance()
		{
			static GameAssembly instance;
			return instance;
		}
		void Init(ApplicationComponents* appComponents);
		ApplicationComponents* GetApplicationComponents() { return m_ApplicationComponents; }
		ScriptFactory& GetFactory() { return m_Factory; }

		// ---------------- Getters ----------------

		static TimeManager* GetTimeManager() { return GetInstance().m_ApplicationComponents->m_TimeManager; };
		static Input* GetInput() { return GetInstance().m_ApplicationComponents->m_InputManager; };
		static SharedPtr<SceneManager> GetSceneManager() { return GetInstance().m_ApplicationComponents->m_SceneManager; }

		//------------------------------------------

	private:
		GameAssembly() : m_Factory() {}
		~GameAssembly() {}

		AK_GAME_ASSEMBLY_API void InitBox2D();

		ScriptFactory m_Factory;
		ApplicationComponents* m_ApplicationComponents = nullptr;
	};

	/* Workaround class so that Application::GetGameAssembly() works inside the DLL. */
	class FakeLoadedAssembly : public LoadedGameAssembly
	{
		virtual bool LoadAssembly(const char* filename) { return true; };
		virtual bool Free() { return true; };

		virtual void Initialize(ApplicationComponents& appComponents) {};
		virtual std::vector<std::string> GetScripts()
		{
			std::vector<std::string> dummy;
			return dummy;
		};

		virtual ScriptableEntity* InstantiateScript(const char* scriptName)
		{
			return GameAssembly::GetInstance().GetFactory().createObject(scriptName);
		};
	};
}

#define RegisterScript(type,name)\
namespace {\
	bool registerScript = Akkad::GameAssembly::GetInstance().GetFactory().RegisterScriptObject<type>(name);\
}

// ----------------- Exports ---------------------

extern "C" AK_GAME_ASSEMBLY_API void InitGameAssembly(Akkad::ApplicationComponents * appComponents);
extern "C" AK_GAME_ASSEMBLY_API std::vector<std::string> & GetScriptsGameAssembly();
extern "C" AK_GAME_ASSEMBLY_API Akkad::ScriptableEntity * InstantiateScriptGameAssembly(const char* scriptName);

// -----------------------------------------------