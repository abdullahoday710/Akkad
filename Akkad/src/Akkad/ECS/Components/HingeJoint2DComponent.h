#pragma once

#include "Akkad/ECS/Entity.h"

#include <box2d/b2_revolute_joint.h>
namespace Akkad {

	struct HingeJoint2DComponent
	{
		Entity bodyA;
		Entity bodyB;

		glm::vec2 localAnchorA = { 0,0 };
		glm::vec2 localAnchorB = { 0,0 };

		bool collideConnected = false;
		b2RevoluteJoint* joint;
	};
}