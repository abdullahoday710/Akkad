#include "GameAssembly.h"

#include <iostream>

namespace Akkad {
	void GameAssembly::Init(ApplicationComponents& appComponents)
	{
		m_ApplicationComponents = &appComponents;

		// Because we have functions inside the engine static library that directly accesses
		// the application singleton, we have to make sure that the dll's Application singleton has the initilized application components.
		Application::GetInstance().m_ApplicationComponents.m_platform = appComponents.m_platform;
		Application::GetInstance().m_ApplicationComponents.m_AssetManager = appComponents.m_AssetManager;
		Application::GetInstance().m_ApplicationComponents.m_TimeManager = appComponents.m_TimeManager;
	}
}


void Init(Akkad::ApplicationComponents& appComponents)
{
	Akkad::GameAssembly::GetInstance().Init(appComponents);
}

std::vector<std::string>& GetScripts()
{
	static std::vector<std::string> scripts;
	scripts.clear();

	auto& factory = Akkad::GameAssembly::GetInstance().GetFactory();

	for (auto it : factory.objectmap)
	{
		scripts.push_back(it.first);
	}

	return scripts;
}

Akkad::ScriptableEntity* InstantiateScript(const char* scriptName)
{
	std::string name = scriptName;

	auto& factory = Akkad::GameAssembly::GetInstance().GetFactory();

	return factory.createObject(scriptName);
}
