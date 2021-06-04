#include "ViewPortPanel.h"
#include "PropertyEditorPanel.h"

#include "Editor/EditorLayer.h"
#include "Editor/Scripting/GameAssemblyHandler.h"

#include <Akkad/Graphics/FrameBuffer.h>
#include <Akkad/ECS/SceneManager.h>
#include <Akkad/Application/Application.h>
#include <Akkad/Input/Input.h>
#include <Akkad/Graphics/Renderer2D.h>
#include <Akkad/ECS/Components/Components.h>
#include <Akkad/Math/Math.h>
#include <Akkad/GUI/GUIText.h>
#include <Akkad/GUI/GUIContainer.h>

#include <imgui.h>
#include <ImGuizmo.h>

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
		
	}

	ViewPortPanel::~ViewPortPanel()
	{
	}

	void ViewPortPanel::DrawImGui()
	{
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

			auto ViewPortPos = ImGui::GetCursorScreenPos();
			ImGui::Image((void*)m_buffer->GetColorAttachmentTexture(), viewportPanelSize, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

			auto viewportRectMin = ImGui::GetItemRectMin();
			auto viewportRectMax = ImGui::GetItemRectMax();
			Graphics::Rect sceneViewportRect;
			sceneViewportRect.SetBounds({ viewportRectMin.x, viewportRectMin.y }, { viewportRectMax.x, viewportRectMax.y });

			m_ViewportRect = sceneViewportRect;
			if (Application::GetSceneManager()->GetActiveScene() != nullptr)
			{
				Application::GetSceneManager()->GetActiveScene()->SetViewportRect(sceneViewportRect);
			}
			
			/* mouse picking entities */
			auto input = Application::GetInputManager();
			static entt::entity lastPickedEntity;
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
						auto pickingBuffer = EditorLayer::GetActiveScene()->GetPickingBuffer();
						if (pickingBuffer != nullptr)
						{
							auto pixel = pickingBuffer->ReadPixels(bufferX, viewportPanelSize.y - bufferY - 1);
							unsigned int entityID = pixel.x;

							entityID -= 1;
							entt::entity entity = (entt::entity)entityID;
							if (EditorLayer::GetActiveScene()->m_Registry.valid(entity))
							{
								lastPickedEntity = entity;
								auto scene = EditorLayer::GetActiveScene();
								Entity e = Entity(entity, scene.get());
								auto tag = e.GetComponent<TagComponent>();
							}
						}
					}
				}
			}

			/* Rendering gizmos */
			if (EditorLayer::GetActiveScene()->m_Registry.valid(lastPickedEntity))
			{
				glm::vec2 viewportRects[2];
				viewportRects[0] = { viewportRectMin.x, viewportRectMin.y};
				viewportRects[1] = { viewportRectMax.x, viewportRectMax.y};

				
				if (m_EditorCamera.GetProjectionType() == CameraProjection::Orthographic)
				{
					ImGuizmo::SetOrthographic(true);
				}
				else
				{
					ImGuizmo::SetOrthographic(false);
				}

				if (IsPlaying)
				{
					ImGuizmo::Enable(false);
				}
				else
				{
					ImGuizmo::Enable(true);
				}

				ImGuizmo::SetDrawlist();
				Entity pickedEntity = Entity(lastPickedEntity, EditorLayer::GetActiveScene().get());
				if (pickedEntity.HasComponent<TransformComponent>())
				{
					auto& comp = EditorLayer::GetActiveScene()->m_Registry.get<TransformComponent>(lastPickedEntity);
					const auto& projection = m_EditorCamera.GetProjection();
					auto view = glm::inverse(m_EditorCamera.GetTransformMatrix());
					auto transform = comp.GetTransformMatrix();

					ImGuizmo::SetRect(viewportRects[0].x, viewportRects[0].y, viewportRects[1].x - viewportRects[0].x, viewportRects[1].y - viewportRects[0].y);
					static auto operation = ImGuizmo::OPERATION::TRANSLATE;
					static auto mode = ImGuizmo::MODE::LOCAL;

					if (input->GetKeyDown(AK_KEY_LEFT_CONTROL) && input->GetKeyDown(AK_KEY_LEFT_ALT) && input->GetKeyDown(AK_KEY_S))
					{
						operation = ImGuizmo::OPERATION::SCALE;
					}

					if (input->GetKeyDown(AK_KEY_LEFT_CONTROL) && input->GetKeyDown(AK_KEY_LEFT_ALT) && input->GetKeyDown(AK_KEY_R))
					{
						operation = ImGuizmo::OPERATION::ROTATE;
					}

					if (input->GetKeyDown(AK_KEY_LEFT_CONTROL) && input->GetKeyDown(AK_KEY_LEFT_ALT) && input->GetKeyDown(AK_KEY_T))
					{
						operation = ImGuizmo::OPERATION::TRANSLATE;
					}
					ImGuizmo::Manipulate(glm::value_ptr(view), glm::value_ptr(projection), operation, mode, glm::value_ptr(transform), nullptr, nullptr);


					if (ImGuizmo::IsUsing())
					{
						glm::vec3 translation, rotation, scale;
						Akkad::DecomposeTransform(transform, translation, rotation, scale);

						glm::vec3 deltaRotation = rotation - comp.GetRotation();
						comp.SetPostion(translation);
						comp.SetScale(scale);
						auto& rotationc = comp.GetRotation();
						rotationc += deltaRotation;
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
		if (GameAssemblyHandler::HasLoadedGameAssembly())
		{
			PropertyEditorPanel::SetActiveEntity({});
			EditorLayer::SaveActiveScene();
			EditorLayer::ReloadGameAssembly();
			IsPlaying = true;

			auto sceneManager = Application::GetSceneManager();

			sceneManager->LoadScene(EditorLayer::GetActiveScenePath());
			sceneManager->GetActiveScene()->Start();
		}

		else
		{
			AK_ERROR("Can't start the scene without a game assembly !, maybe the assembly was never compiled or it was deleted.");
		}

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
			auto sceneManager = Application::GetSceneManager();
			sceneManager->GetActiveScene()->SetViewportSize({ m_buffer->GetDescriptor().width, m_buffer->GetDescriptor().height });
			sceneManager->GetActiveScene()->SetViewportRect(m_ViewportRect);
			Renderer2D::BeginScene(m_EditorCamera, m_EditorCamera.GetTransformMatrix());
			sceneManager->GetActiveScene()->RenderPickingBuffer2D();

			m_buffer->Bind();
			sceneManager->GetActiveScene()->Render2D();
			sceneManager->GetActiveScene()->RenderGUI();
			m_buffer->Unbind();
		}

		else
		{
			m_EditorCamera.Update();
			EditorLayer::GetActiveScene()->SetViewportSize({ m_buffer->GetDescriptor().width, m_buffer->GetDescriptor().height });
			EditorLayer::GetActiveScene()->SetViewportRect(m_ViewportRect);
			Renderer2D::BeginScene(m_EditorCamera, m_EditorCamera.GetTransformMatrix());

			EditorLayer::GetActiveScene()->RenderPickingBuffer2D();

			m_buffer->Bind();
			EditorLayer::GetActiveScene()->Render2D();
			EditorLayer::GetActiveScene()->RenderGUI();
			m_buffer->Unbind();
		}

	}

}