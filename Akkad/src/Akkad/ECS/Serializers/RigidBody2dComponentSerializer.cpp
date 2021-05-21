#include "RigidBody2dComponentSerializer.h"

#include "Akkad/ECS/Components/RigidBody2dComponent.h"
namespace Akkad {

	void RigidBody2dComponentSerializer::Serialize(Entity entity, json& entity_data)
	{
		auto& body2dcomponent = entity.GetComponent<RigidBody2dComponent>();
		entity_data["RigidBody2D"]["Friction"] = body2dcomponent.friction;
		entity_data["RigidBody2D"]["Density"] = body2dcomponent.density;

		switch (body2dcomponent.shape)
		{
		case BodyShape::POLYGON_SHAPE:
		{
			entity_data["RigidBody2D"]["Shape"] = "Polygon";
			break;
		}

		}

		switch (body2dcomponent.type)
		{

		case BodyType::STATIC:
		{
			entity_data["RigidBody2D"]["Type"] = "Static";
			break;
		}

		case BodyType::DYNAMIC:
		{
			entity_data["RigidBody2D"]["Type"] = "Dynamic";
			break;
		}

		}
	}

	void RigidBody2dComponentSerializer::Deserialize(Entity entity, json& component_data)
	{
		BodyType type;
		BodyShape shape;
		float density = component_data["Density"];
		float friction = component_data["Friction"];

		if (component_data["Type"] == "Static")
		{
			type = BodyType::STATIC;
		}

		else if (component_data["Type"] == "Dynamic")
		{
			type = BodyType::DYNAMIC;
		}

		if (component_data["Shape"] == "Polygon")
		{
			shape = BodyShape::POLYGON_SHAPE;
		}

		entity.AddComponent<RigidBody2dComponent>(type, shape, density, friction);
	}

}