#include "GameViewPanel.h"
#include "ViewPortPanel.h"

#include "Editor/EditorLayer.h"

#include <Akkad/Application/Application.h>
#include <Akkad/ECS/SceneManager.h>
#include <Akkad/Graphics/FrameBuffer.h>

#include <imgui.h>
namespace Akkad {
	using namespace Graphics;
	bool GameViewPanel::showPanel;
	
	GameViewPanel::GameViewPanel()
	{
		FrameBufferDescriptor descriptor;
		descriptor.width = 800;
		descriptor.height = 800;
		descriptor.ColorAttachmentFormat = TextureFormat::RGB16;

		m_buffer = Application::GetRenderPlatform()->CreateFrameBuffer(descriptor);
	}

	void GameViewPanel::DrawImGui()
	{
		FrameBufferDescriptor desc;
		desc.width = 800;
		desc.height = 800;

		if (ImGui::Begin("Game view", &showPanel))
		{
			auto panelSize = ImGui::GetContentRegionAvail();
			m_AspectRatio = panelSize.x / panelSize.y;
			m_buffer->SetSize(panelSize.x, panelSize.y);
			m_buffer->Bind();
			ImGui::Image((void*)m_buffer->GetColorAttachmentTexture(), panelSize, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
			m_buffer->Unbind();
			IsSelected = true;
		}
		else
		{
			IsSelected = false;
		}
		ImGui::End();
	}

	void GameViewPanel::OnClose()
	{
		showPanel = false;
	}

	void GameViewPanel::RenderScene()
	{
		ViewPortPanel* viewport = (ViewPortPanel*)PanelManager::GetPanel("viewport");
		m_buffer->Bind();

		if (viewport->IsPlaying)
		{
			auto sceneManager = Application::GetSceneManager();
			sceneManager->GetActiveScene()->BeginRenderer2D(m_AspectRatio);
			sceneManager->GetActiveScene()->Render2D();
			sceneManager->GetActiveScene()->RenderGUI();

		}

		else
		{
			auto scene = EditorLayer::GetActiveScene();
			scene->BeginRenderer2D(m_AspectRatio);
			scene->Render2D();
			scene->RenderGUI();
		}

		m_buffer->Unbind();
	}
}