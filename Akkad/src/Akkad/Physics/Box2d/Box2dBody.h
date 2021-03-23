#pragma once
#include "Akkad/core.h"

#include <box2d/box2d.h>
#include <glm/glm.hpp>

namespace Akkad {

	namespace Graphics {
		class VertexBuffer;
		class Shader;
	}

	enum class BodyType {
		STATIC, DYNAMIC
	};

	enum class BodyShape {
		POLYGON_SHAPE
	};

	struct BodySettings {
		glm::vec2 position;
		float rotation;
		BodyType type;
		BodyShape shape;

		float density;
		float friction;

		float halfX;
		float halfY;

	};

	class Box2dBody
	{
	public:
		Box2dBody() {};
		Box2dBody(b2Body* body);

		bool IsValid();
		glm::vec2 GetPosition();
		float GetRotation();

		void DrawBoundingBox();
	private:
		b2Body* m_Body = nullptr;
		friend class RigidBody2dComponent;

	protected:
		SharedPtr<Graphics::VertexBuffer> m_VertexBuffer;
		static SharedPtr<Graphics::Shader> m_DebugShader;

	};
}


