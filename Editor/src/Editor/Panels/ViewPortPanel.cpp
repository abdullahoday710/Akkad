#include "ViewPortPanel.h"
#include "PropertyEditorPanel.h"

#include "Editor/EditorLayer.h"

#include <Akkad/Graphics/FrameBuffer.h>
#include <Akkad/ECS/SceneManager.h>
#include <Akkad/Application/Application.h>
#include <Akkad/Input/Input.h>
#include <Akkad/Graphics/Renderer2D.h>
#include <Akkad/ECS/Components/TagComponent.h>

#include <imgui.h>
namespace Akkad {
	using namespace Graphics;

	bool ViewPortPanel::showPanel;

	ViewPortPanel::ViewPortPanel() : m_EditorCamera()
	{
		FrameBufferDescriptor sceneBufferDescriptor;
		sceneBufferDescriptor.width = 800;
		sceneBufferDescriptor.height = 800;
		sceneBufferDescriptor.ColorAttachmentFormat = TextureFormat::RGB16;
		m_buffer = Application::GetRenderPlatform()->CreateFrameBuffer(sceneBufferDescriptor);

		FrameBufferDescriptor pickingBufferDescriptor;
		pickingBufferDescriptor.width = 800;
		pickingBufferDescriptor.height = 800;
		pickingBufferDescriptor.ColorAttachmentFormat = TextureFormat::RGB32_FLOAT;
		m_PickingBuffer = Application::GetRenderPlatform()->CreateFrameBuffer(pickingBufferDescriptor);
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
			m_ViewportAspectRatio = viewportPanelSize.x / viewportPanelSize.y;

			m_buffer->SetSize(viewportPanelSize.x, viewportPanelSize.y);
			m_PickingBuffer->SetSize(viewportPanelSize.x, viewportPanelSize.y);

			auto ViewPortPos = ImGui::GetCursorScreenPos();
			ImGui::Image((void*)m_buffer->GetColorAttachmentTexture(), viewportPanelSize, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

			auto viewportRectMin = ImGui::GetItemRectMin();
			auto viewportRectMax = ImGui::GetItemRectMax();

			auto input = Application::GetInputManager();
			if (input->GetMouseDown(MouseButtons::LEFT))
			{
				int mouseX = input->GetMouseX();
				int mouseY = input->GetMouseY();

				if (mouseX < viewportRectMax.x && mouseY < viewportRectMax.y)
				{
					if (mouseX > viewportRectMin.x && mouseY > viewportRectMin.y)
					{
						int bufferX = mouseX - (int)ViewPortPos.x;
						int bufferY = mouseY - (int)ViewPortPos.y;

						auto pixel = m_PickingBuffer->ReadPixels(bufferX, viewportPanelSize.y - bufferY - 1);
						unsigned int entityID = pixel.x;

						entityID -= 1;
						entt::entity entity = (entt::entity)entityID;
						if (EditorLayer::GetActiveScene()->m_Registry.valid(entity))
						{
							auto comp = EditorLayer::GetActiveScene()->m_Registry.get<TagComponent>(entity);
							std::cout << "Entity tag is" << comp.Tag << std::endl;
						}

					}
				}
			}
			m_buffer->Unbind();
		ImGui::End();
	}

	void ViewPortPanel::OnClose()
	{
		showPanel = false;
	}

	void ViewPortPanel::OnScenePlay()
	{
		PropertyEditorPanel::SetActiveEntity({});
		EditorLayer::SaveActiveScene();
		EditorLayer::ReloadGameAssembly();
		IsPlaying = true;

		auto sceneManager = Application::GetSceneManager();

		sceneManager->LoadScene(EditorLayer::GetActiveScenePath());
		sceneManager->GetActiveScene()->Start();
	}

	void ViewPortPanel::OnSceneStop()
	{
		PropertyEditorPanel::SetActiveEntity({});
		IsPlaying = false;
		auto sceneManager = Application::GetSceneManager();

		sceneManager->GetActiveScene()->Stop();
		sceneManager->LoadScene(EditorLayer::GetActiveScenePath());
	}

	void ViewPortPanel::RenderScene()
	{
		m_EditorCamera.SetAspectRatio(m_ViewportAspectRatio);

		if (IsPlaying)
		{
			m_buffer->Bind();

			auto sceneManager = Application::GetSceneManager();
			Renderer2D::BeginScene(m_EditorCamera, m_EditorCamera.GetTransformMatrix());
			sceneManager->GetActiveScene()->Render2D();

			m_buffer->Unbind();
		}

		else
		{
			m_EditorCamera.Update();
			Renderer2D::BeginScene(m_EditorCamera, m_EditorCamera.GetTransformMatrix());

			m_PickingBuffer->Bind();
			EditorLayer::GetActiveScene()->RenderPickingBuffer2D();
			m_PickingBuffer->Unbind();

			m_buffer->Bind();
			EditorLayer::GetActiveScene()->Render2D();
			m_buffer->Unbind();
		}

	}

}