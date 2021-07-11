#include "Box2dWorld.h"

#include "Akkad/ECS/Scene.h"
#include "Akkad/ECS/Entity.h"
#include "Akkad/Application/Application.h"
#include "Akkad/Asset/AssetManager.h"
#include "Akkad/Graphics/Renderer2D.h"

namespace Akkad {

	Box2dWorld::Box2dWorld()
	{
		b2Vec2 gravity = { m_Gravity.x, m_Gravity.y };
		m_World = CreateSharedPtr<b2World>(gravity);
	}

	Box2dWorld::Box2dWorld(glm::vec2 gravity)
	{
		b2Vec2 b2gravity = {gravity.x, gravity.y};
		m_World = CreateSharedPtr<b2World>(b2gravity);
	}

	Box2dWorld::~Box2dWorld()
	{
	}

	Box2dBody Box2dWorld::CreateBody(BodySettings settings, Scene* scene, uint32_t entityid)
	{
		b2BodyDef bodyDef;
		Entity* entity = new Entity((entt::entity)entityid, scene);

		bodyDef.position.Set(settings.position.x, settings.position.y);
		bodyDef.angle = settings.rotation;
		bodyDef.userData.pointer = reinterpret_cast<uintptr_t>(entity);
		
		switch (settings.type)
		{
		case BodyType::STATIC:
			bodyDef.type = b2_staticBody;
			break;
		case BodyType::DYNAMIC:
			bodyDef.type = b2_dynamicBody;
			break;
		}

		b2Body* body = m_World->CreateBody(&bodyDef);
		b2FixtureDef fixtureDef;

		fixtureDef.density = settings.density;
		fixtureDef.friction = settings.friction;
		switch (settings.shape)
		{
		case BodyShape::POLYGON_SHAPE:
		{
			b2PolygonShape polygon;
			polygon.SetAsBox(settings.halfX, settings.halfY);
			fixtureDef.shape = &polygon;
			body->CreateFixture(&fixtureDef);
			break;
		}

		}

		return Box2dBody(body);
	}

	void Box2dWorld::SetContactListener(Box2dContactListener* listener)
	{
		m_World->SetContactListener(listener);
	}

	void Box2dWorld::SetDebugDraw(Box2dDraw* draw)
	{
		m_World->SetDebugDraw(draw);
	}

	void Box2dWorld::Step()
	{
		float timeStep = 1.0f / 60.0f;
		int32 velocityIterations = 6;
		int32 positionIterations = 2;

		m_World->Step(timeStep, velocityIterations, positionIterations);
	}

	void Box2dWorld::DestroyJoint(b2Joint* joint)
	{
		m_World->DestroyJoint(joint);
	}

	void Box2dWorld::Clear()
	{
		m_World.reset(new b2World({m_Gravity.x, m_Gravity.y}));
	}
}