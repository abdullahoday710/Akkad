#include "HingeJoint2DSerializer.h"

#include "Akkad/ECS/Components/HingeJoint2DComponent.h"
namespace Akkad {

	void HingeJoint2DSerializer::Serialize(Entity entity, json& entity_data)
	{
		auto& hinge = entity.GetComponent<HingeJoint2DComponent>();

		entity_data["HingeJoint2D"]["BodyA"] = hinge.bodyA._GetHandle();
		entity_data["HingeJoint2D"]["BodyB"] = hinge.bodyB._GetHandle();

		entity_data["HingeJoint2D"]["LocalAnchorA"][0] = hinge.localAnchorA.x;
		entity_data["HingeJoint2D"]["LocalAnchorA"][1] = hinge.localAnchorA.y;

		entity_data["HingeJoint2D"]["LocalAnchorB"][0] = hinge.localAnchorB.x;
		entity_data["HingeJoint2D"]["LocalAnchorB"][1] = hinge.localAnchorB.y;

		entity_data["HingeJoint2D"]["CollideConnected"] = hinge.collideConnected;
	}

	void HingeJoint2DSerializer::Deserialize(Entity entity, json& component_data)
	{
		auto& hinge = entity.AddComponent<HingeJoint2DComponent>();

		uint32_t entity_id_A = component_data["BodyA"];
		uint32_t entity_id_B = component_data["BodyB"];
		glm::vec2 localAnchorA;
		glm::vec2 localAnchorB;
		localAnchorA.x = component_data["LocalAnchorA"][0];
		localAnchorA.y = component_data["LocalAnchorA"][1];

		localAnchorB.x = component_data["LocalAnchorB"][0];
		localAnchorB.y = component_data["LocalAnchorB"][1];

		bool collideConnected = component_data["CollideConnected"];

		hinge.bodyA = { (entt::entity)entity_id_A, entity._GetScene() };
		hinge.bodyB = { (entt::entity)entity_id_B, entity._GetScene() };
		hinge.localAnchorA = localAnchorA;
		hinge.localAnchorB = localAnchorB;
		hinge.collideConnected = collideConnected;

	}
}