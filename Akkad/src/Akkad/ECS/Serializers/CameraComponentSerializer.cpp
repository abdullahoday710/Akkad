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

		entity_data["CameraComponent"]["Camera"]["ClearColor"]["R"] = camera.GetClearColor().r;
		entity_data["CameraComponent"]["Camera"]["ClearColor"]["G"] = camera.GetClearColor().g;
		entity_data["CameraComponent"]["Camera"]["ClearColor"]["B"] = camera.GetClearColor().b;
	}

	void CameraComponentSerializer::Deserialize(Entity entity, json& component_data)
	{
		CameraProjection projtype;
		glm::vec3 clearColor = {};
		if (component_data["Camera"]["Projection"] == "Orthographic")
		{
			projtype = CameraProjection::Orthographic;
		}

		else if (component_data["Camera"]["Projection"] == "Perspective")
		{
			projtype = CameraProjection::Perspective;
		}

		clearColor.r = component_data["Camera"]["ClearColor"]["R"];
		clearColor.g = component_data["Camera"]["ClearColor"]["G"];
		clearColor.b = component_data["Camera"]["ClearColor"]["B"];

		auto& camera = entity.AddComponent<CameraComponent>(projtype);
		camera.camera.SetClearColor(clearColor);
	}
}