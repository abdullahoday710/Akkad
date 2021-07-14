#include "GameAssembly.h"

#include <Akkad/Graphics/Renderer2D.h>
#include <iostream>

#include <box2d/b2_world.h>
namespace Akkad {

	void GameAssembly::Init(ApplicationComponents& appComponents)
	{
		m_ApplicationComponents = &appComponents;

		// Because we have functions inside the engine static library that directly accesses
		// the application singleton, we have to make sure that the dll's Application singleton has the initilized application components.
		Application::GetInstance().m_ApplicationComponents.m_platform = appComponents.m_platform;
		Application::GetInstance().m_ApplicationComponents.m_AssetManager = appComponents.m_AssetManager;
		Application::GetInstance().m_ApplicationComponents.m_TimeManager = appComponents.m_TimeManager;
		Application::GetInstance().m_ApplicationComponents.m_Window = appComponents.m_Window;

		// Apply the workaround class, not sure if I will keep it this way.
		Application::GetInstance().m_LoadedGameAssembly = new FakeLoadedAssembly();

		Graphics::Renderer2D::s_Instance = *appComponents.m_Renderer2D;

		InitBox2D();
	}

	// HACK : Box2d have some static variables that can only be initialized when creating a temporary world.
	void GameAssembly::InitBox2D()
	{
		b2World* world = new b2World({ 10,10 });

		b2BodyDef myBodyDef;
		myBodyDef.type = b2_dynamicBody;
		myBodyDef.position.Set(0, 0);
		myBodyDef.angle = 0;
		b2PolygonShape boxShape;
		boxShape.SetAsBox(1, 1);

		b2FixtureDef boxFixtureDef;
		boxFixtureDef.shape = &boxShape;
		boxFixtureDef.density = 1;
		b2Body* dynamicBody = world->CreateBody(&myBodyDef);
		b2Body* dynamicBody1 = world->CreateBody(&myBodyDef);
		dynamicBody->CreateFixture(&boxFixtureDef);
		dynamicBody1->CreateFixture(&boxFixtureDef);
		world->Step(10, 10, 10);

		delete world;
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
