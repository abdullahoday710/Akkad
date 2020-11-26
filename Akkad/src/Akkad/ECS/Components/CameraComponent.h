#pragma once

#include "Akkad/core.h"
#include "Akkad/Graphics/Camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Akkad {

	struct CameraComponent {
		Camera camera;
		bool isActive = true;

		CameraComponent(CameraProjection cameraProjection)
			:camera(cameraProjection)
		{
		}

		CameraComponent(CameraProjection cameraProjection, float aspectRatio)
			:camera(cameraProjection, aspectRatio)
		{
		}
	};
}