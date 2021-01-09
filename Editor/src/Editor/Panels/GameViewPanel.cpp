#include "GameViewPanel.h"
#include "Editor/EditorLayer.h"

#include <imgui.h>
namespace Akkad {
	bool GameViewPanel::showPanel;
	
	void GameViewPanel::DrawImGui()
	{
		FrameBufferDescriptor desc;
		desc.width = 800;
		desc.height = 800;

		static auto framebuffer = Application::GetRenderPlatform()->CreateFrameBuffer(desc);

		if (ImGui::Begin("Game view", &showPanel))
		{
			auto panelSize = ImGui::GetContentRegionAvail();
			framebuffer->SetSize(panelSize.x, panelSize.y);
			framebuffer->Bind();
			EditorLayer::GetActiveScene()->BeginRenderer2D();
			EditorLayer::GetActiveScene()->Render2D();
			ImGui::Image((void*)framebuffer->GetColorAttachmentTexture(), panelSize, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

			framebuffer->Unbind();
		}
		ImGui::End();
	}

	void GameViewPanel::OnClose()
	{

	}
}