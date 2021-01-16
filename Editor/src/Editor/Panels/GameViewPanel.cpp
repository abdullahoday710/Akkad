#include "GameViewPanel.h"
#include "ViewPortPanel.h"

#include "Editor/EditorLayer.h"

#include <imgui.h>
namespace Akkad {
	using namespace Graphics;
	bool GameViewPanel::showPanel;
	
	GameViewPanel::GameViewPanel()
	{
		FrameBufferDescriptor descriptor;
		descriptor.width = 800;
		descriptor.height = 800;

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
			m_buffer->SetSize(panelSize.x, panelSize.y);
			m_buffer->Bind();
			ImGui::Image((void*)m_buffer->GetColorAttachmentTexture(), panelSize, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
			m_buffer->Unbind();
		}
		ImGui::End();
	}

	void GameViewPanel::OnClose()
	{
	}
	void GameViewPanel::RenderScene()
	{
		ViewPortPanel* viewport = (ViewPortPanel*)PanelManager::GetPanel("viewport");
		m_buffer->Bind();

		EditorLayer::GetActiveScene()->BeginRenderer2D();
		EditorLayer::GetActiveScene()->Render2D();
		m_buffer->Unbind();
	}
}