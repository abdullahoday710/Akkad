#pragma once

#include "Akkad/Physics/Box2d/Box2dBody.h"
namespace Akkad {

	struct RigidBody2dComponent
	{
		RigidBody2dComponent(BodyType Intype, BodyShape Inshape, float Indensity, float Infriction) {
			type = Intype;
			shape = Inshape;
			density = Indensity;
			friction = Infriction;
		};
		RigidBody2dComponent() {}
		Box2dBody body;

		BodyType type = BodyType::STATIC;
		BodyShape shape = BodyShape::POLYGON_SHAPE;

		b2Body* GetBody() { return body.m_Body; }

		float density = 0.0f;
		float friction = 0.0f;

	};
}