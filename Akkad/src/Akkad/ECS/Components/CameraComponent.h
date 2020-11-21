#pragma once
#include "Akkad/core.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Akkad {

	enum class CameraType {
		Orthographic, Perspective
	};

	struct CameraComponent {

		glm::mat4 projection = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 viewProjection = projection * view;
		bool isActive = true;
		CameraType type;

		CameraComponent(CameraType cameraType, float aspectRatio) {
			type = cameraType;

			switch (cameraType)
			{
				case Akkad::CameraType::Orthographic:
				{
					projection = glm::ortho(-aspectRatio, aspectRatio, -1.0f, 1.0f, -1.0f, 1.0f);
					break;
				}

				case Akkad::CameraType::Perspective:
				{
					break;
				}
			}
		}

		void RecalculateViewProjectionMatrix(glm::vec3& position)
		{
			glm::mat4 transform = glm::translate(glm::mat4(1.0f), position);
			view = glm::inverse(transform);

			viewProjection = projection * view;
		}
	};
}