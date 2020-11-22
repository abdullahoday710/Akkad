#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Akkad {

	enum class CameraProjection {
		Orthographic, Perspective
	};

	class Camera {
	public:
		Camera(float aspectRatio) {
			m_ProjectionMatrix = glm::ortho(-aspectRatio, aspectRatio, -1.0f, 1.0f, -1.0f, 1.0f);
			m_AspectRatio = aspectRatio;
		}

		Camera(CameraProjection projectionType, float aspectRatio) {
			switch (projectionType)
			{
				case Akkad::CameraProjection::Orthographic:
					{
						m_ProjectionMatrix = glm::ortho(-aspectRatio, aspectRatio, -1.0f, 1.0f, -1.0f, 1.0f);
						m_AspectRatio = aspectRatio;
						break;
					}
				case Akkad::CameraProjection::Perspective:
					{
						m_ProjectionMatrix = glm::mat4(1.0f); //TODO
						m_AspectRatio = aspectRatio;
						break;
					}
			}
		}

		glm::mat4& GetProjection() { return m_ProjectionMatrix; }
		
		float m_AspectRatio = 0.0f;
	private:
		glm::mat4 m_ProjectionMatrix = glm::mat4(1.0f);
	};

}