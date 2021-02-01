#include "GameAssembly.h"

#include <Akkad/Application/Application.h>
#include <iostream>

namespace Akkad {
	void GameAssembly::Init(ApplicationComponents& appComponents)
	{
		m_ApplicationComponents = &appComponents;
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

