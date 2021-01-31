#pragma once

#include <Akkad/Graphics/Camera.h>

namespace Akkad {

	class EditorCamera : public Camera
	{
	public:
		EditorCamera() {};

		EditorCamera(float aspectRatio);

		EditorCamera(CameraProjection projectionType);

		EditorCamera(CameraProjection projectionType, float aspectRatio);

		void Update();

		glm::mat4 GetTransformMatrix() {
			return m_Transform;
		}

		void RecalculateTransform() {
			m_Transform = glm::mat4(1.0f);
			m_Transform = glm::translate(m_Transform, m_Position);
		}

	private:
		glm::vec3 m_Position = glm::vec3(0.0f);
		glm::mat4 m_Transform = glm::mat4(1.0f);
	};
}