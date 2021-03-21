#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Akkad {

	enum class CameraProjection {
		Orthographic, Perspective
	};

	class Camera {
	public:
		Camera() {
			m_AspectRatio = 1.0f;
			m_ProjectionMatrix = glm::ortho(-1.0f - m_ZoomLevel, 1.0f + m_ZoomLevel, -1.0f - m_ZoomLevel, 1.0f + m_ZoomLevel, -1.0f, 1.0f);
			m_ProjectionType = CameraProjection::Orthographic;
		}

		Camera(float aspectRatio) {
			m_ProjectionMatrix = glm::ortho(-aspectRatio - m_ZoomLevel, aspectRatio + m_ZoomLevel, -1.0f - m_ZoomLevel, 1.0f + m_ZoomLevel, -1.0f, 1.0f);
			m_AspectRatio = aspectRatio;
			m_ProjectionType = CameraProjection::Orthographic;
		}

		Camera(CameraProjection projectionType) {
			m_AspectRatio = 1.0f;
			SetProjection(projectionType);
		}

		Camera(CameraProjection projectionType, float aspectRatio) {
			m_AspectRatio = aspectRatio;
			SetProjection(projectionType);
		}

		void SetProjection(CameraProjection projectionType) {
			m_ProjectionType = projectionType;

			switch (projectionType)
			{
			case Akkad::CameraProjection::Orthographic:
			{
				//m_ProjectionMatrix = glm::ortho(-m_AspectRatio, m_AspectRatio, -1.0f, 1.0f, -1.0f, 1.0f);
				m_ProjectionMatrix = glm::ortho(-m_AspectRatio - m_ZoomLevel, m_AspectRatio + m_ZoomLevel, -1.0f - m_ZoomLevel, 1.0f + m_ZoomLevel, -1.0f, 1.0f);
				break;
			}
			case Akkad::CameraProjection::Perspective:
			{
				m_ProjectionMatrix = glm::mat4(1.0f); //TODO
				break;
			}
			}
		}

		void SetAspectRatio(float ratio) {
			m_AspectRatio = ratio;
			SetProjection(m_ProjectionType);
		}

		float GetAspectRatio() { return m_AspectRatio; }
		glm::mat4& GetProjection() { return m_ProjectionMatrix; }

		CameraProjection GetProjectionType() { return m_ProjectionType; }
		
		float m_ZoomLevel = 0.0f;
	private:
		float m_AspectRatio = 0.0f;
		glm::mat4 m_ProjectionMatrix = glm::mat4(1.0f);
		CameraProjection m_ProjectionType;
	};

}