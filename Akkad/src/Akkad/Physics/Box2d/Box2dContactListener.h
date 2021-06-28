#pragma once

#include <box2d/box2d.h>
namespace Akkad {
	class Box2dContactListener : public b2ContactListener
	{
		virtual void BeginContact(b2Contact* contact) override;
		virtual void EndContact(b2Contact* contact) override;
	};
}