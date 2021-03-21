#pragma once
#include "Box2dBody.h"
#include "Akkad/core.h"

#include <box2d/box2d.h>
#include <glm/glm.hpp>

namespace Akkad {

	class Box2dWorld
	{
	public:
		Box2dWorld();
		Box2dWorld(glm::vec2 gravity);
		~Box2dWorld();

		Box2dBody CreateBody(BodySettings settings);
		void Step();

		void Clear();

	private:
		SharedPtr<b2World> m_World;
		glm::vec2 m_Gravity = {0.0f, -10.0f};
	};
}


