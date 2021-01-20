#include "ViewPortPanel.h"

#include "Editor/EditorLayer.h"
#include "Akkad/Graphics/FrameBuffer.h"
#include "Akkad/ECS/SceneManager.h"
#include <imgui.h>
namespace Akkad {
	using namespace Graphics;

	bool ViewPortPanel::showPanel;

	ViewPortPanel::ViewPortPanel() : m_EditorCamera()
	{
		FrameBufferDescriptor descriptor;
		descriptor.width = 800;
		descriptor.height = 800;
		m_buffer = Application::GetRenderPlatform()->CreateFrameBuffer(descriptor);
	}

	ViewPortPanel::~ViewPortPanel()
	{
	}

	void ViewPortPanel::DrawImGui()
	{
		FrameBufferDescriptor descriptor;
		descriptor.width = 800;
		descriptor.height = 800;

		ImGui::Begin("Viewport");
			if (!IsPlaying)
			{
				if (ImGui::Button("Play"))
				{
					OnScenePlay();
				}
			}
			else
			{
				if (ImGui::Button("Stop"))
				{
					OnSceneStop();
				}
			}

			m_buffer->Bind();
			ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
			m_buffer->SetSize(viewportPanelSize.x, viewportPanelSize.y);
			ImGui::Image((void*)m_buffer->GetColorAttachmentTexture(), viewportPanelSize, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
			m_buffer->Unbind();
		ImGui::End();
	}

	void ViewPortPanel::OnClose()
	{
	}

	void ViewPortPanel::OnScenePlay()
	{
		IsPlaying = true;

		auto sceneManager = Application::GetSceneManager();

		sceneManager->LoadScene(EditorLayer::GetActiveScenePath());
		sceneManager->GetActiveScene()->Start();
	}

	void ViewPortPanel::OnSceneStop()
	{
		IsPlaying = false;
		auto sceneManager = Application::GetSceneManager();

		sceneManager->GetActiveScene()->Stop();
		sceneManager->LoadScene(EditorLayer::GetActiveScenePath());
	}

	void ViewPortPanel::RenderScene()
	{
		m_buffer->Bind();

		if (IsPlaying)
		{
			auto sceneManager = Application::GetSceneManager();
			Renderer2D::BeginScene(m_EditorCamera, m_EditorCamera.GetTransformMatrix());
			sceneManager->GetActiveScene()->Render2D();
		}

		else
		{
			m_EditorCamera.Update();
			Renderer2D::BeginScene(m_EditorCamera, m_EditorCamera.GetTransformMatrix());
			EditorLayer::GetActiveScene()->Render2D();
		}

		m_buffer->Unbind();
	}

}