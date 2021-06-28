#include "Box2dContactListener.h"

#include "Akkad/ECS/Entity.h"
#include "Akkad/ECS/Components/Components.h"

namespace Akkad {

	void Box2dContactListener::BeginContact(b2Contact* contact)
	{
		Entity* entityA = (Entity*)contact->GetFixtureA()->GetBody()->GetUserData().pointer;
		Entity* entityB = (Entity*)contact->GetFixtureB()->GetBody()->GetUserData().pointer;

		if (entityA->HasComponent<ScriptComponent>())
		{
			auto scriptA = entityA->GetComponent<ScriptComponent>();
			scriptA.Instance->OnColliderEnter2D(*entityB);
		}

		if (entityB->HasComponent<ScriptComponent>())
		{
			auto scriptB = entityB->GetComponent<ScriptComponent>();
			scriptB.Instance->OnColliderEnter2D(*entityA);
		}
	}

	void Box2dContactListener::EndContact(b2Contact* contact)
	{
		Entity* entityA = (Entity*)contact->GetFixtureA()->GetBody()->GetUserData().pointer;
		Entity* entityB = (Entity*)contact->GetFixtureB()->GetBody()->GetUserData().pointer;

		if (entityA->HasComponent<ScriptComponent>())
		{
			auto scriptA = entityA->GetComponent<ScriptComponent>();
			if (scriptA.Instance != nullptr)
			{
				scriptA.Instance->OnColliderExit2D(*entityB);
			}
		}

		if (entityB->HasComponent<ScriptComponent>())
		{
			auto scriptB = entityB->GetComponent<ScriptComponent>();
			if (scriptB.Instance != nullptr)
			{
				scriptB.Instance->OnColliderExit2D(*entityA);
			}
		}
	}
}