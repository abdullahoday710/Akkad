#pragma once
#include <Akkad/Application/Time.h>
#include <Akkad/Input/KeyCodes.h>
#include <Akkad/Input/Input.h>
#include <Akkad/Graphics/Camera.h>

namespace Akkad {

	class EditorCamera : public Camera
	{
	public:
		EditorCamera() {};

		EditorCamera(float aspectRatio) : Camera(aspectRatio)
		{
			m_Transform = glm::translate(m_Transform, m_Position);
		}

		EditorCamera(CameraProjection projectionType) : Camera(projectionType)
		{
			m_Transform = glm::translate(m_Transform, m_Position);
		}

		EditorCamera(CameraProjection projectionType, float aspectRatio) : Camera(projectionType, aspectRatio)
		{
			m_Transform = glm::translate(m_Transform, m_Position);
		}

		void Update() {

			if (Input::GetKeyDown(AK_KEY_W))
			{
				m_Position.y += 0.5f * Time::GetDeltaTime();
				RecalculateTransform();
			}

			if (Input::GetKeyDown(AK_KEY_A))
			{
				m_Position.x -= 0.5f * Time::GetDeltaTime();
				RecalculateTransform();
			}

			if (Input::GetKeyDown(AK_KEY_S))
			{
				m_Position.y -= 0.5f * Time::GetDeltaTime();
				RecalculateTransform();
			}

			if (Input::GetKeyDown(AK_KEY_D))
			{
				m_Position.x += 0.5f * Time::GetDeltaTime();
				RecalculateTransform();
			}
		}
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