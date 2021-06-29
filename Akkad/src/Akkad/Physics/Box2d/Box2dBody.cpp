#include "Box2dBody.h"
#include "Akkad/Graphics/Renderer2D.h"
#include "Akkad/Application/Application.h"
#include "Akkad/Asset/AssetManager.h"

namespace Akkad {
	Box2dBody::Box2dBody(b2Body* body)
	{
		m_Body = body;
	}

	bool Box2dBody::IsValid()
	{
		if (m_Body != nullptr)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	glm::vec2 Box2dBody::GetPosition()
	{
		AK_ASSERT(IsValid(), "invalid body !");
		b2Vec2 pos = m_Body->GetPosition();

		return {pos.x, pos.y};
	}

	float Box2dBody::GetRotation()
	{
		return m_Body->GetAngle();
	}
}