#include <Akkad/Application/Application.h>
#include <Akkad/Application/TimeManager.h>

#include <Akkad/Input/KeyCodes.h>
#include <Akkad/Input/Input.h>
#include "EditorCamera.h"

namespace Akkad {

	EditorCamera::EditorCamera(float aspectRatio) : Camera(aspectRatio)
	{
		m_Transform = glm::translate(m_Transform, m_Position);
	}


	EditorCamera::EditorCamera(CameraProjection projectionType) : Camera(projectionType)
	{
		m_Transform = glm::translate(m_Transform, m_Position);
	}

	EditorCamera::EditorCamera(CameraProjection projectionType, float aspectRatio) : Camera(projectionType, aspectRatio)
	{
		m_Transform = glm::translate(m_Transform, m_Position);
	}

	void EditorCamera::Update()
	{
		auto time = Application::GetTimeManager();
		auto input = Application::GetInputManager();

		if (input->GetKeyDown(AK_KEY_W))
		{
			m_Position.y += 0.5f * time->GetDeltaTime();
			RecalculateTransform();
		}

		if (input->GetKeyDown(AK_KEY_A))
		{
			m_Position.x -= 0.5f * time->GetDeltaTime();
			RecalculateTransform();
		}

		if (input->GetKeyDown(AK_KEY_S))
		{
			m_Position.y -= 0.5f * time->GetDeltaTime();
			RecalculateTransform();
		}

		if (input->GetKeyDown(AK_KEY_D))
		{
			m_Position.x += 0.5f * time->GetDeltaTime();
			RecalculateTransform();
		}

		if (input->GetKeyDown(AK_KEY_KP_SUBTRACT))
		{
			m_ZoomLevel += 1.0f * time->GetDeltaTime();
		}
		if (input->GetKeyDown(AK_KEY_KP_ADD))
		{
			m_ZoomLevel -= 1.0f * time->GetDeltaTime();
		}
	}

}