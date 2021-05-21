#include "CameraComponentSerializer.h"

#include "Akkad/ECS/Components/CameraComponent.h"
namespace Akkad {

	void CameraComponentSerializer::Serialize(Entity entity, json& entity_data)
	{
		auto& cameraComponent = entity.GetComponent<CameraComponent>();
		auto& camera = cameraComponent.camera;

		switch (camera.GetProjectionType())
		{
		case CameraProjection::Orthographic:
			entity_data["CameraComponent"]["Camera"]["Projection"] = "Orthographic";
			break;
		case CameraProjection::Perspective:
			entity_data["CameraComponent"]["Camera"]["Projection"] = "Perspective";
			break;
		}
	}

	void CameraComponentSerializer::Deserialize(Entity entity, json& component_data)
	{
		CameraProjection projtype;
		if (component_data["Camera"]["Projection"] == "Orthographic")
		{
			projtype = CameraProjection::Orthographic;
		}

		else if (component_data["Camera"]["Projection"] == "Perspective")
		{
			projtype = CameraProjection::Perspective;
		}

		entity.AddComponent<CameraComponent>(projtype);
	}
}