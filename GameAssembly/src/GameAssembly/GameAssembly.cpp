#include "GameAssembly.h"

#include <iostream>
#include <Akkad/Application/Application.h>
namespace Akkad {
	void GameAssembly::Init(Application& app)
	{
		m_Application = &app;
	}
}


void Init(Akkad::Application& app)
{
	Akkad::GameAssembly::GetInstance().Init(app);
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

