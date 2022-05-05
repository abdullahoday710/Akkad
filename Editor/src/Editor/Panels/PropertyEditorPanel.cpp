#include "PropertyEditorPanel.h"
#include "MaterialEditorPanel.h"
#include "SpriteSelectorPanel.h"

#include "Editor/EditorLayer.h"
#include "Editor/Serializers/MaterialSerializer.h"
#include "Editor/Scripting/GameAssemblyHandler.h"

#include <Akkad/Application/Application.h>
#include <Akkad/Scripting/LoadedGameAssembly.h>
#include <Akkad/ECS/Components/Components.h>
#include <Akkad/Asset/AssetManager.h>
#include <Akkad/Graphics/SortingLayer2D.h>

#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>

namespace Akkad {
	bool PropertyEditorPanel::showPanel;

	bool AddComponentsOpen = false;
	Entity PropertyEditorPanel::m_ActiveEntity;

	PropertyEditorPanel::PropertyEditorPanel()
	{
	}

	PropertyEditorPanel::~PropertyEditorPanel()
	{
	}

	template <typename T>
	bool DrawComponentContextMenu(Entity e)
	{
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::Button("Delete"))
			{
				e.RemoveComponent<T>();
				ImGui::EndPopup();
				return true;
			}

			ImGui::EndPopup();
			return false;
		}
	}

	void PropertyEditorPanel::DrawImGui()
	{
		ImGui::Begin("Property Editor", &showPanel);

		ImGui::Text("Properties: ");
		if (m_ActiveEntity.IsValid())
		{
			DrawTagComponent();

			if (m_ActiveEntity.HasComponent<TransformComponent>())
			{
				DrawTransformComponent();
			}

			if (m_ActiveEntity.HasComponent<SpriteRendererComponent>())
			{
				DrawSpriteRendererComponent();
			}

			if (m_ActiveEntity.HasComponent<CameraComponent>())
			{
				DrawCameraComponent();
			}

			if (m_ActiveEntity.HasComponent<ScriptComponent>())
			{
				DrawScriptComponent();
			}

			if (m_ActiveEntity.HasComponent<RigidBody2dComponent>())
			{
				DrawRigidBody2dComponent();
			}

			if (m_ActiveEntity.HasComponent<RectTransformComponent>())
			{
				DrawRectTransformComponent();
			}

			if (m_ActiveEntity.HasComponent<GUITextComponent>())
			{
				DrawGUITextComponent();
			}

			if (m_ActiveEntity.HasComponent<GUIContainerComponent>())
			{
				DrawGUIContainerComponent();
			}

			if (m_ActiveEntity.HasComponent<GUIButtonComponent>())
			{
				DrawGUIButtonComponent();
			}

			if (m_ActiveEntity.HasComponent<AnimatedSpriteRendererComponent>())
			{
				DrawAnimatedSpriteRendererComponent();
			}
			if (m_ActiveEntity.HasComponent<HingeJoint2DComponent>())
			{
				DrawHingeJoint2DComponent();
			}
			if (m_ActiveEntity.HasComponent<LineRendererComponent>())
			{
				DrawLineRendererComponent();
			}
			if (m_ActiveEntity.HasComponent<ColoredSpriteRendererComponent>())
			{
				DrawColoredSpriteRendererComponent();
			}
			if (m_ActiveEntity.HasComponent<GUIPanelComponent>())
			{
				DrawGUIPanelComponent();
			}
			if (m_ActiveEntity.HasComponent<GUICheckBoxComponent>())
			{
				DrawGUICheckBoxComponent();
			}
			if (m_ActiveEntity.HasComponent<GUISliderComponent>())
			{
				DrawGUISliderComponent();
			}
			if (m_ActiveEntity.HasComponent<GUITextInputComponent>())
			{
				DrawGUITextInputComponent();
			}

			DrawAddComponent();
		}
		

		ImGui::End();
	}

	void PropertyEditorPanel::OnClose()
	{
		showPanel = false;
	}

	void PropertyEditorPanel::DrawAddComponent()
	{
		if (ImGui::Button("Add component"))
		{
			AddComponentsOpen = !AddComponentsOpen;
		}

		if (AddComponentsOpen)
		{
			ImGui::Separator();
			if (ImGui::TreeNode("2D"))
			{
				if (ImGui::Button("Sprite Renderer"))
				{
					if (!m_ActiveEntity.HasComponent<SpriteRendererComponent>())
					{
						m_ActiveEntity.AddComponent<SpriteRendererComponent>();
					}
				}

				if (ImGui::Button("Line Renderer"))
				{
					if (!m_ActiveEntity.HasComponent<LineRendererComponent>())
					{
						m_ActiveEntity.AddComponent<LineRendererComponent>();
					}
				}

				if (ImGui::Button("Animated Sprite Renderer"))
				{
					if (!m_ActiveEntity.HasComponent<AnimatedSpriteRendererComponent>())
					{
						m_ActiveEntity.AddComponent<AnimatedSpriteRendererComponent>();
					}
				}

				if (ImGui::Button("Rigidbody 2D"))
				{
					if (!m_ActiveEntity.HasComponent<RigidBody2dComponent>())
					{
						m_ActiveEntity.AddComponent<RigidBody2dComponent>();
					}
				}


				if (ImGui::Button("Hinge joint 2D"))
				{
					if (!m_ActiveEntity.HasComponent<HingeJoint2DComponent>())
					{
						m_ActiveEntity.AddComponent<HingeJoint2DComponent>();
					}
				}

				if (ImGui::Button("Camera"))
				{

					if (!m_ActiveEntity.HasComponent<CameraComponent>())
					{
						m_ActiveEntity.AddComponent<CameraComponent>(CameraProjection::Orthographic);
					}
				}
				ImGui::TreePop();
			}

			if (ImGui::Button("Script"))
			{
				if (!m_ActiveEntity.HasComponent<ScriptComponent>())
				{
					m_ActiveEntity.AddComponent<ScriptComponent>("");
				}
			}
		
		}
	}

	void PropertyEditorPanel::DrawTagComponent()
	{
		ImGui::SetNextItemOpen(true);
		if (ImGui::TreeNode("Tag"))
		{
			auto& tag = m_ActiveEntity.GetComponent<TagComponent>();
			std::string& str = tag.Tag;
			ImGui::InputText("Tag", &str);

			ImGui::TreePop();
		}
		
	}

	void PropertyEditorPanel::DrawTransformComponent()
	{
		ImGui::SetNextItemOpen(true);
		if (ImGui::TreeNode("Transform"))
		{
			auto& transform = m_ActiveEntity.GetComponent<TransformComponent>();
			glm::vec3 position = transform.GetPosition();
			glm::vec3 rotation = transform.GetRotation();
			glm::vec3 scale = transform.GetScale();

			if (ImGui::DragFloat3("Position", glm::value_ptr(position)))
			{
				transform.SetPostion(position - transform.m_ParentPosition);
			}
			if (ImGui::DragFloat3("Rotation", glm::value_ptr(rotation)))
			{
				transform.SetRotation(rotation - transform.m_ParentRotation);
			}
			if (ImGui::DragFloat3("Scale", glm::value_ptr(scale)))
			{
				transform.SetScale(scale);
			}
			ImGui::TreePop();
		}


	}

	void PropertyEditorPanel::DrawSpriteRendererComponent()
	{
		ImGui::SetNextItemOpen(true);
		if (ImGui::TreeNode("Sprite Renderer"))
		{
			if (DrawComponentContextMenu<SpriteRendererComponent>(m_ActiveEntity))
			{
				ImGui::TreePop();
				return;
			}
			auto& sprite = m_ActiveEntity.GetComponent<SpriteRendererComponent>();

			auto material = sprite.sprite.GetMaterial();

			ImGui::Text("Material :");

			if (material != nullptr && material->isValid())
			{
				if (ImGui::Button(material->GetName().c_str()))
				{
					auto desc = Application::GetAssetManager()->GetDescriptorByID(sprite.materialID);
					MaterialEditorPanel::SetActiveMaterial(material, sprite.materialID);
					PanelManager::AddPanel(new MaterialEditorPanel());
				}

				
			}
			else
			{
				std::string buf;
				ImGui::InputText("Material", &buf, ImGuiInputTextFlags_ReadOnly);
			}

			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET_DRAG_DROP"))
				{
					const char* id = (const char*)payload->Data;
					AssetDescriptor desc = Application::GetAssetManager()->GetDescriptorByID(id);

					if (desc.assetType == AssetType::MATERIAL)
					{
						sprite.materialID = desc.assetID;
						sprite.sprite.SetMaterial(desc.absolutePath);
					}
				}
				ImGui::EndDragDropTarget();
			}

			auto layers = SortingLayer2DHandler::GetRegisteredLayers();
			static int item_current_idx = 0;
			if (ImGui::BeginCombo("Sorting Layer",sprite.sprite.GetSortingLayer().c_str()))
			{
				for (int i = 0; i < layers.size(); i++)
				{
					const bool is_selected = (item_current_idx == i);
					if (ImGui::Selectable(layers[i].name.c_str(), is_selected))
					{
						item_current_idx = i;
						sprite.sprite.SetSortingLayer(layers[i].name);
					}

					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}

			if (sprite.sprite.IsValid())
			{
				if (sprite.sprite.IsUsingTilemap())
				{
					float row = sprite.sprite.GetTileRow();
					if (ImGui::InputFloat("Tile row", &row))
					{
						sprite.sprite.SetTileRow(row);
					}

					float col = sprite.sprite.GetTileColoumn();
					if (ImGui::InputFloat("Tile coloumn", &col))
					{
						sprite.sprite.SetTileColoumn(col);
					}

					if (ImGui::Button("Select sprite from tilemap"))
					{
						PanelManager::AddPanel(new SpriteSelectorPanel());

						SpriteSelectorPanel::SetActiveEntity(m_ActiveEntity);
					}
				}
			}
			
			ImGui::TreePop();
		}
	}

	void PropertyEditorPanel::DrawCameraComponent()
	{
		ImGui::SetNextItemOpen(true);
		if (ImGui::TreeNode("Camera"))
		{

			auto& camera = m_ActiveEntity.GetComponent<CameraComponent>();
			ImGui::Text("Camera :");
			ImGui::Checkbox("Active", &camera.isActive);
			auto clearColor = camera.camera.GetClearColor();
			if (ImGui::ColorEdit3("clear color", glm::value_ptr(clearColor)))
			{
				camera.camera.SetClearColor(clearColor);
			}
		
			ImGui::TreePop();
		}
	}

	void PropertyEditorPanel::DrawScriptComponent()
	{
		ImGui::SetNextItemOpen(true);
		if (ImGui::TreeNode("Script"))
		{
			if (GameAssemblyHandler::HasLoadedGameAssembly())
			{

				auto gameAssembly = Application::GetGameAssembly();
				std::vector<std::string> scriptNames;

				for (auto it : gameAssembly->GetScripts())
				{
					scriptNames.push_back(it);
				}

				static int item_current_idx = 0;
				const char* combo_label = scriptNames[item_current_idx].c_str();

				auto& script = m_ActiveEntity.GetComponent<ScriptComponent>();
				if (script.ScriptName.empty())
				{
					script.ScriptName = scriptNames[item_current_idx];
				}

				else
				{
					ptrdiff_t pos = std::find(scriptNames.begin(), scriptNames.end(), script.ScriptName) - scriptNames.begin();
					if (pos <= scriptNames.size())
					{
						item_current_idx = pos;
					}
				}

				if (ImGui::BeginCombo("Script", combo_label))
				{
					for (int i = 0; i < scriptNames.size(); i++)
					{
						const bool is_selected = (item_current_idx == i);
						if (ImGui::Selectable(scriptNames[i].c_str(), is_selected))
						{
							item_current_idx = i;
							script.ScriptName = scriptNames[i];
						}

						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}
			}

			else
			{
				if (ImGui::BeginCombo("Script", "Game assembly not loaded !"))
				{
					ImGui::EndCombo();
				}
				
			}

			ImGui::TreePop();
		}

	}

	std::string BodyTypeToStr(BodyType type)
	{
		switch (type)
		{
		case BodyType::STATIC:
			return "Static";
		case BodyType::DYNAMIC:
			return "Dynamic";
		case BodyType::KINEMATIC:
			return "Kinematic";
		}
	}
	void PropertyEditorPanel::DrawRigidBody2dComponent()
	{
		ImGui::SetNextItemOpen(true);
		if (ImGui::TreeNode("Rigid Body"))
		{
			if (DrawComponentContextMenu<RigidBody2dComponent>(m_ActiveEntity))
			{
				ImGui::TreePop();
				return;
			}
			auto& rigidBody = m_ActiveEntity.GetComponent<RigidBody2dComponent>();
			ImGui::Text("Rigid body :");

			const char* body_types[] = { "Static", "Dynamic", "Kinematic" };
			std::string bodytypestr = BodyTypeToStr(rigidBody.type);
			static const char* current_body_type = bodytypestr.c_str();

			if (ImGui::BeginCombo("Body type", current_body_type))
			{
				for (int n = 0; n < IM_ARRAYSIZE(body_types); n++)
				{
					bool is_selected = (current_body_type == body_types[n]);
					if (ImGui::Selectable(body_types[n], is_selected))
					{
						current_body_type = body_types[n];
						if (current_body_type == "Static")
						{
							rigidBody.type = BodyType::STATIC;
						}
						if (current_body_type == "Dynamic")
						{
							rigidBody.type = BodyType::DYNAMIC;
						}
						if (current_body_type == "Kinematic")
						{
							rigidBody.type = BodyType::KINEMATIC;
						}
					}
					if (is_selected)
					{
						ImGui::SetItemDefaultFocus();
					}
				}
				ImGui::EndCombo();
			}

			rigidBody.shape = BodyShape::POLYGON_SHAPE; // TODO : make the shapes selectable and add more shapes.

			ImGui::InputFloat("Density", &rigidBody.density);
			ImGui::InputFloat("Friction", &rigidBody.friction);

			ImGui::TreePop();
		}
	}
	std::string TextFittingModeToStr(GUI::GUIText::FittingMode mode)
	{
		switch (mode)
		{
		case GUI::GUIText::FittingMode::SCALE_TO_FIT:
			return "Scale to fit";
		case GUI::GUIText::FittingMode::KEEP_FONT_SIZE:
			return "Keep font size";
		}
	}

	GUI::GUIText::FittingMode GetFittingModeFromStr(std::string mode)
	{
		if (mode == "Scale to fit")
		{
			return GUI::GUIText::FittingMode::SCALE_TO_FIT;
		}
		if (mode == "Keep font size")
		{
			return GUI::GUIText::FittingMode::KEEP_FONT_SIZE;
		}
	}
	void PropertyEditorPanel::DrawGUITextComponent()
	{
		ImGui::SetNextItemOpen(true);
		auto& uitext = m_ActiveEntity.GetComponent<GUITextComponent>();

		if (ImGui::TreeNode("gui text"))
		{
			std::string text = uitext.text;
			if (ImGui::InputText("Text", &text))
			{
				uitext.text = text;
			}
			const char* fittingmodes[] = { "Scale to fit", "Keep font size" };
			std::string current_fitting_mode = TextFittingModeToStr(uitext.fittingMode);
			if (ImGui::BeginCombo("Fitting mode", current_fitting_mode.c_str()))
			{
				for (int n = 0; n < IM_ARRAYSIZE(fittingmodes); n++)
				{
					bool is_selected = (current_fitting_mode == fittingmodes[n]);
					if (ImGui::Selectable(fittingmodes[n], is_selected))
					{
						current_fitting_mode = fittingmodes[n];
						uitext.fittingMode = GetFittingModeFromStr(current_fitting_mode);
					}
					if (is_selected)
					{
						ImGui::SetItemDefaultFocus();
					}
				}
				ImGui::EndCombo();
			}

			ImGui::Separator();
			ImGui::Text("Text alignment");
			if (ImGui::Button("Left"))
			{
				uitext.alignment = GUI::GUIText::Alignment::LEFT;
			}
			if (ImGui::Button("Center"))
			{
				uitext.alignment = GUI::GUIText::Alignment::CENTER;
			}
			
			if (uitext.fittingMode == GUI::GUIText::FittingMode::KEEP_FONT_SIZE)
			{
				int ftsize = uitext.fontSize;
				if (ImGui::InputInt("font size", &ftsize))
				{
					if (ftsize > 10)
					{
						uitext.fontSize = ftsize;
					}
				}
			}

			glm::vec3 color = uitext.color;
			if (ImGui::ColorEdit3("Text color", glm::value_ptr(color)))
			{
				uitext.color = color;
			}

		ImGui::TreePop();
		}
	}

	void PropertyEditorPanel::DrawGUIContainerComponent()
	{
		ImGui::SetNextItemOpen(true);
		auto& containerComponent = m_ActiveEntity.GetComponent<GUIContainerComponent>();
		glm::vec2 size = containerComponent.container.GetScreenSize();
		if (ImGui::TreeNode("container"))
		{
			ImGui::InputFloat2("size", glm::value_ptr(size));
			ImGui::TreePop();
		}
	}

	std::string ConstraintTypeToStr_(GUI::ConstraintType type)
	{
		switch (type)
		{
		case GUI::ConstraintType::CENTER_CONSTRAINT:
			return "Center";
		case GUI::ConstraintType::RELATIVE_CONSTRAINT:
			return "Relative";
		case GUI::ConstraintType::PREVIOUS_CHILD_MAX_CONSTRAINT:
			return "Align with previous child max";
		case GUI::ConstraintType::PREVIOUS_CHILD_MIN_CONSTRAINT:
			return "Align with previous child min";
		case GUI::ConstraintType::PREVIOUS_CHILD_CENTER_CONSTRAINT:
			return "Align with previous child center";
		case GUI::ConstraintType::PIXEL_CONSTRAINT:
			return "Pixel";
		case GUI::ConstraintType::ASPECT_CONSTRAINT:
			return "Aspect";
		default:
			break;
		}
	}

	std::string AnchorTypeToStr_(GUI::AnchorType type)
	{
		switch (type)
		{
		case GUI::AnchorType::TOP_LEFT:
			return "top left";
		case GUI::AnchorType::TOP_RIGHT:
			return "top right";
		case GUI::AnchorType::BOTTOM_LEFT:
			return "bottom left";
		case GUI::AnchorType::BOTTOM_RIGHT:
			return "bottom right";
		default:
			break;
		}
	}

	void PropertyEditorPanel::DrawRectTransformComponent()
	{
		ImGui::SetNextItemOpen(true);
		auto& rectTransformComp = m_ActiveEntity.GetComponent<RectTransformComponent>();

		if (ImGui::TreeNode("Rect Transform"))
		{
			glm::vec2 pos = rectTransformComp.GetRect().GetPosition();
			glm::vec2 size = { rectTransformComp.GetRect().GetWidth(), rectTransformComp.GetRect().GetHeight() };

			auto wConstraint = rectTransformComp.rect.GetWidthConstraint();
			auto hConstraint = rectTransformComp.rect.GetHeightConstraint();

			auto xConstraint = rectTransformComp.rect.GetXConstraint();
			auto yConstraint = rectTransformComp.rect.GetYConstraint();

			const char* size_constraints[] = { "Relative", "Pixel", "Aspect" };
			std::string selected_width_constraint = ConstraintTypeToStr_(wConstraint.type);

			if (ImGui::BeginCombo("Width constraint type", selected_width_constraint.c_str()))
			{
				for (int n = 0; n < IM_ARRAYSIZE(size_constraints); n++)
				{
					bool is_selected = (selected_width_constraint == size_constraints[n]);
					if (ImGui::Selectable(size_constraints[n], is_selected))
					{
						selected_width_constraint = size_constraints[n];

						if (selected_width_constraint == "Relative")
						{
							rectTransformComp.rect.SetWidthConstraint({ GUI::ConstraintType::RELATIVE_CONSTRAINT, wConstraint.constraintValue });
						}

						if (selected_width_constraint == "Pixel")
						{
							rectTransformComp.rect.SetWidthConstraint({ GUI::ConstraintType::PIXEL_CONSTRAINT, wConstraint.constraintValue });
						}

						if (selected_width_constraint == "Aspect")
						{
							rectTransformComp.rect.SetWidthConstraint({ GUI::ConstraintType::ASPECT_CONSTRAINT, wConstraint.constraintValue });
						}
					}
					if (is_selected)
					{
						ImGui::SetItemDefaultFocus();
					}
				}
				ImGui::EndCombo();
			}

			if (ImGui::DragFloat("Width constraint", &wConstraint.constraintValue))
			{
				rectTransformComp.rect.SetWidthConstraint(wConstraint);
			}

			ImGui::Separator();

			std::string selected_height_constraint = ConstraintTypeToStr_(hConstraint.type);
			if (ImGui::BeginCombo("Height constraint type", ConstraintTypeToStr_(hConstraint.type).c_str()))
			{
				for (int n = 0; n < IM_ARRAYSIZE(size_constraints); n++)
				{
					bool is_selected = (selected_height_constraint == size_constraints[n]);
					if (ImGui::Selectable(size_constraints[n], is_selected))
					{
						selected_height_constraint = size_constraints[n];

						if (selected_height_constraint == "Relative")
						{
							rectTransformComp.rect.SetHeightConstraint({ GUI::ConstraintType::RELATIVE_CONSTRAINT, wConstraint.constraintValue });
						}

						if (selected_height_constraint == "Pixel")
						{
							rectTransformComp.rect.SetHeightConstraint({ GUI::ConstraintType::PIXEL_CONSTRAINT, wConstraint.constraintValue });
						}

						if (selected_height_constraint == "Aspect")
						{
							rectTransformComp.rect.SetHeightConstraint({ GUI::ConstraintType::ASPECT_CONSTRAINT, wConstraint.constraintValue });
						}
					}
					if (is_selected)
					{
						ImGui::SetItemDefaultFocus();
					}
				}

				ImGui::EndCombo();
			}

			if (ImGui::DragFloat("Height constraint", &hConstraint.constraintValue))
			{
				rectTransformComp.rect.SetHeightConstraint(hConstraint);
			}

			ImGui::Separator();

			const char* position_constraints[] = { "Relative", "Pixel", "Center", "Align with previous child max", "Align with previous child min", "Align with previous child center" };
			std::string selected_x_constraint = ConstraintTypeToStr_(xConstraint.type);

			if (ImGui::BeginCombo("X constraint type", selected_x_constraint.c_str()))
			{
				for (int n = 0; n < IM_ARRAYSIZE(position_constraints); n++)
				{
					bool is_selected = (selected_x_constraint == position_constraints[n]);
					if (ImGui::Selectable(position_constraints[n], is_selected))
					{
						selected_x_constraint = position_constraints[n];

						if (selected_x_constraint == "Relative")
						{
							rectTransformComp.rect.SetXConstraint({ GUI::ConstraintType::RELATIVE_CONSTRAINT, wConstraint.constraintValue });
						}

						if (selected_x_constraint == "Pixel")
						{
							rectTransformComp.rect.SetXConstraint({ GUI::ConstraintType::PIXEL_CONSTRAINT, wConstraint.constraintValue });
						}

						if (selected_x_constraint == "Center")
						{
							rectTransformComp.rect.SetXConstraint({ GUI::ConstraintType::CENTER_CONSTRAINT, wConstraint.constraintValue });
						}

						if (selected_x_constraint == "Align with previous child max")
						{
							rectTransformComp.rect.SetXConstraint({ GUI::ConstraintType::PREVIOUS_CHILD_MAX_CONSTRAINT, wConstraint.constraintValue });
						}
						if (selected_x_constraint == "Align with previous child min")
						{
							rectTransformComp.rect.SetXConstraint({ GUI::ConstraintType::PREVIOUS_CHILD_MIN_CONSTRAINT, wConstraint.constraintValue });
						}
						if (selected_x_constraint == "Align with previous child center")
						{
							rectTransformComp.rect.SetXConstraint({ GUI::ConstraintType::PREVIOUS_CHILD_CENTER_CONSTRAINT, wConstraint.constraintValue });
						}
					}
					if (is_selected)
					{
						ImGui::SetItemDefaultFocus();
					}
				}
				ImGui::EndCombo();
			}

			if (ImGui::DragFloat("X constraint", &xConstraint.constraintValue))
			{
				rectTransformComp.rect.SetXConstraint(xConstraint);
			}

			ImGui::Separator();

			std::string selected_y_constraint = ConstraintTypeToStr_(yConstraint.type);

			if (ImGui::BeginCombo("Y constraint type", ConstraintTypeToStr_(yConstraint.type).c_str()))
			{
				for (int n = 0; n < IM_ARRAYSIZE(position_constraints); n++)
				{
					bool is_selected = (selected_y_constraint == position_constraints[n]);
					if (ImGui::Selectable(position_constraints[n], is_selected))
					{
						selected_y_constraint = position_constraints[n];

						if (selected_y_constraint == "Relative")
						{
							rectTransformComp.rect.SetYConstraint({ GUI::ConstraintType::RELATIVE_CONSTRAINT, wConstraint.constraintValue });
						}

						if (selected_y_constraint == "Pixel")
						{
							rectTransformComp.rect.SetYConstraint({ GUI::ConstraintType::PIXEL_CONSTRAINT, wConstraint.constraintValue });
						}

						if (selected_y_constraint == "Center")
						{
							rectTransformComp.rect.SetYConstraint({ GUI::ConstraintType::CENTER_CONSTRAINT, wConstraint.constraintValue });
						}
						if (selected_y_constraint == "Align with previous child max")
						{
							rectTransformComp.rect.SetYConstraint({ GUI::ConstraintType::PREVIOUS_CHILD_MAX_CONSTRAINT, wConstraint.constraintValue });
						}
						if (selected_y_constraint == "Align with previous child min")
						{
							rectTransformComp.rect.SetYConstraint({ GUI::ConstraintType::PREVIOUS_CHILD_MIN_CONSTRAINT, wConstraint.constraintValue });
						}
						if (selected_y_constraint == "Align with previous child center")
						{
							rectTransformComp.rect.SetYConstraint({ GUI::ConstraintType::PREVIOUS_CHILD_CENTER_CONSTRAINT, wConstraint.constraintValue });
						}
					}
					if (is_selected)
					{
						ImGui::SetItemDefaultFocus();
					}
				}
				ImGui::EndCombo();
			}

			if (ImGui::DragFloat("Y constraint", &yConstraint.constraintValue))
			{
				rectTransformComp.rect.SetYConstraint(yConstraint);
			}

			auto anchorType = rectTransformComp.rect.GetAnchorType();
			std::string selected_anchor_type = AnchorTypeToStr_(anchorType);

			const char* anchor_types[] = { "top left", "top right", "bottom left", "bottom right" };
			if (ImGui::BeginCombo("anchor type", AnchorTypeToStr_(anchorType).c_str()))
			{
				for (int n = 0; n < IM_ARRAYSIZE(anchor_types); n++)
				{
					bool is_selected = (selected_anchor_type == anchor_types[n]);
					if (ImGui::Selectable(anchor_types[n], is_selected))
					{
						selected_anchor_type = anchor_types[n];

						if (selected_anchor_type == "top left")
						{
							rectTransformComp.rect.SetAnchorType(GUI::AnchorType::TOP_LEFT);
						}

						if (selected_anchor_type == "top right")
						{
							rectTransformComp.rect.SetAnchorType(GUI::AnchorType::TOP_RIGHT);
						}
						if (selected_anchor_type == "bottom left")
						{
							rectTransformComp.rect.SetAnchorType(GUI::AnchorType::BOTTOM_LEFT);
						}
						if (selected_anchor_type == "bottom right")
						{
							rectTransformComp.rect.SetAnchorType(GUI::AnchorType::BOTTOM_RIGHT);
						}
						if (is_selected)
						{
							ImGui::SetItemDefaultFocus();
						}
					}
				}
				ImGui::EndCombo();
			}
			ImGui::TreePop();
		}
	}

	void PropertyEditorPanel::DrawGUIButtonComponent()
	{
		ImGui::SetNextItemOpen(true);
		auto& guibutton = m_ActiveEntity.GetComponent<GUIButtonComponent>();
		if (ImGui::TreeNode("gui button"))
		{
			glm::vec3 color = guibutton.button.GetColor();
			if (ImGui::ColorEdit3("color", glm::value_ptr(color)))
			{
				guibutton.button.SetColor(color);
			}
			ImGui::TreePop();
		}
	}

	void PropertyEditorPanel::DrawAnimatedSpriteRendererComponent()
	{
		ImGui::SetNextItemOpen(true);
		auto& animatedSprite = m_ActiveEntity.GetComponent<AnimatedSpriteRendererComponent>();
		if (ImGui::TreeNode("animated sprite"))
		{
			auto material = animatedSprite.sprite.GetMaterial();

			ImGui::Text("Material :");

			if (material != nullptr && material->isValid())
			{
				if (ImGui::Button(material->GetName().c_str()))
				{
					auto desc = Application::GetAssetManager()->GetDescriptorByID(animatedSprite.materialID);
					MaterialEditorPanel::SetActiveMaterial(material, animatedSprite.materialID);
					PanelManager::AddPanel(new MaterialEditorPanel());
				}


			}
			else
			{
				std::string buf;
				ImGui::InputText("Material", &buf, ImGuiInputTextFlags_ReadOnly);
			}

			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET_DRAG_DROP"))
				{
					const char* id = (const char*)payload->Data;
					AssetDescriptor desc = Application::GetAssetManager()->GetDescriptorByID(id);

					if (desc.assetType == AssetType::MATERIAL)
					{
						animatedSprite.materialID = desc.assetID;
						animatedSprite.sprite.SetMaterial(desc.absolutePath);
					}
				}
				ImGui::EndDragDropTarget();
			}

			auto layers = SortingLayer2DHandler::GetRegisteredLayers();
			static int item_current_idx = 0;
			if (ImGui::BeginCombo("Sorting Layer", animatedSprite.sprite.GetSortingLayer().c_str()))
			{
				for (int i = 0; i < layers.size(); i++)
				{
					const bool is_selected = (item_current_idx == i);
					if (ImGui::Selectable(layers[i].name.c_str(), is_selected))
					{
						item_current_idx = i;
						animatedSprite.sprite.SetSortingLayer(layers[i].name);
					}

					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}

			if (ImGui::ListBoxHeader("Animations"))
			{
				int id = 0;
				for (auto& it : animatedSprite.sprite.m_Animations)
				{
					bool isActive = false;
					if (it.first == animatedSprite.sprite.m_ActiveAnimation)
					{
						isActive = true;
					}
					if (ImGui::Button(it.first.c_str()))
					{

					}
					ImGui::SameLine();

					ImGui::PushID(id);
					if (ImGui::Checkbox("active", &isActive))
					{
						animatedSprite.sprite.m_ActiveAnimation = it.first;
					}
					ImGui::PopID();

					id++;
				}
				ImGui::ListBoxFooter();
			}

			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET_DRAG_DROP"))
				{
					const char* id = (const char*)payload->Data;
					AssetDescriptor desc = Application::GetAssetManager()->GetDescriptorByID(id);

					if (desc.assetType == AssetType::SPRITE_ANIMATION)
					{
						auto animation = animatedSprite.sprite.AddAnimation(desc.assetID);
					}
				}
				ImGui::EndDragDropTarget();
			}
			
			ImGui::TreePop();
		}
	}

	void PropertyEditorPanel::DrawHingeJoint2DComponent()
	{
		ImGui::SetNextItemOpen(true);
		auto& hinge = m_ActiveEntity.GetComponent<HingeJoint2DComponent>();
		if (ImGui::TreeNode("Hinge joint 2D"))
		{
			std::string tagA;
			std::string tagB;

			if (hinge.bodyA.IsValid())
			{
				tagA = hinge.bodyA.GetComponent<TagComponent>().Tag;
			}

			if (hinge.bodyB.IsValid())
			{
				tagB = hinge.bodyB.GetComponent<TagComponent>().Tag;
			}

			ImGui::InputText("Body A", &tagA, ImGuiInputTextFlags_ReadOnly);
			if (ImGui::BeginDragDropTarget())
			{
				auto scene = EditorLayer::GetActiveScene();
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("HIERARCHY_DRAG_DROP"))
				{
					size_t* entity_id = (size_t*)payload->Data;
					Entity bodyA = { (entt::entity)*entity_id, scene.get() };
					if (bodyA != hinge.bodyB)
					{
						hinge.bodyA = bodyA;
					}

				}
				ImGui::EndDragDropTarget();
			}
			ImGui::InputText("Body B", &tagB, ImGuiInputTextFlags_ReadOnly);
			if (ImGui::BeginDragDropTarget())
			{
				auto scene = EditorLayer::GetActiveScene();
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("HIERARCHY_DRAG_DROP"))
				{
					size_t* entity_id = (size_t*)payload->Data;
					Entity bodyB = { (entt::entity) * entity_id, scene.get() };
					if (bodyB != hinge.bodyA)
					{
						hinge.bodyB = bodyB;
					}

				}
				ImGui::EndDragDropTarget();
			}

			ImGui::InputFloat2("Local anchor A", glm::value_ptr(hinge.localAnchorA));
			ImGui::InputFloat2("Local anchor B", glm::value_ptr(hinge.localAnchorB));

			ImGui::Checkbox("Collide connected", &hinge.collideConnected);

			ImGui::Checkbox("Enable motor", &hinge.enableMotor);
			ImGui::InputFloat("Motor speed", &hinge.motorSpeed);
			ImGui::InputFloat("Max motor torque", &hinge.maxMotorTorque);
			ImGui::TreePop();
		}
	}

	void PropertyEditorPanel::DrawLineRendererComponent()
	{
		ImGui::SetNextItemOpen(true);
		auto& lineRenderer = m_ActiveEntity.GetComponent<LineRendererComponent>();
		glm::vec3 color = lineRenderer.color;
		if (ImGui::InputFloat3("Color", glm::value_ptr(color)))
		{
			lineRenderer.color = color;
		}
		
		if (ImGui::ListBoxHeader("Lines"))
		{
			int id = 0;
			for (auto& it : lineRenderer.lines)
			{
				ImGui::PushID(id);
				ImGui::InputFloat4("Line position", glm::value_ptr(it));
				ImGui::PopID();
				id++;
			}
			ImGui::ListBoxFooter();
		}

		if (ImGui::Button("+"))
		{
			lineRenderer.lines.push_back({});
		}
		ImGui::SameLine();
		if (ImGui::Button("-"))
		{
			lineRenderer.lines.pop_back();
		}
	}

	void PropertyEditorPanel::DrawColoredSpriteRendererComponent()
	{
		ImGui::SetNextItemOpen(true);
		auto& coloredSprite = m_ActiveEntity.GetComponent<ColoredSpriteRendererComponent>();
		glm::vec3 color = coloredSprite.color;

		if (ImGui::ColorEdit3("Color", glm::value_ptr(color)))
		{
			coloredSprite.color = color;
		}
	}

	void PropertyEditorPanel::DrawGUIPanelComponent()
	{
		ImGui::SetNextItemOpen(true);
		auto& panel = m_ActiveEntity.GetComponent<GUIPanelComponent>();
		glm::vec3 color = panel.panel.GetColor();
		if (ImGui::ColorEdit3("Color", glm::value_ptr(color)))
		{
			panel.panel.SetColor(color);
		}

		bool transparent = panel.panel.IsTransparent();

		if (ImGui::Checkbox("Is transparent", &transparent))
		{
			panel.panel.SetTransparent(transparent);
		}
	}

	void PropertyEditorPanel::DrawGUICheckBoxComponent()
	{
		ImGui::SetNextItemOpen(true);
		auto& box = m_ActiveEntity.GetComponent<GUICheckBoxComponent>();

		glm::vec3 boxColor = box.box.GetBoxColor();
		glm::vec3 markColor = box.box.GetMarkColor();

		bool isActive = box.box.IsChecked();

		if (ImGui::ColorEdit3("Box color", glm::value_ptr(boxColor)))
		{
			box.box.SetBoxColor(boxColor);
		}
		if (ImGui::ColorEdit3("Mark color", glm::value_ptr(markColor)))
		{
			box.box.SetMarkColor(markColor);
		}

		if (ImGui::Checkbox("Is checked", &isActive))
		{
			box.box.SetCheckStatus(isActive);
		}
	}

	void PropertyEditorPanel::DrawGUISliderComponent()
	{
		ImGui::SetNextItemOpen(true);
		auto& slider = m_ActiveEntity.GetComponent<GUISliderComponent>();

		glm::vec3 sliderColor = slider.slider.GetSliderColor();
		glm::vec3 knobColor = slider.slider.GetKnobColor();

		if (ImGui::ColorEdit3("Slider color", glm::value_ptr(sliderColor)))
		{
			slider.slider.SetSliderColor(sliderColor);
		}
		if (ImGui::ColorEdit3("Knob color", glm::value_ptr(knobColor)))
		{
			slider.slider.SetKnobColor(knobColor);
		}
	}

	void PropertyEditorPanel::DrawGUITextInputComponent()
	{
		auto& textinput = m_ActiveEntity.GetComponent<GUITextInputComponent>();

		glm::vec3 textinputColor = textinput.textinput.GetTextInputColor();
		glm::vec3 textColor = textinput.textinput.GetTextColor();

		if (ImGui::ColorEdit3("Text Input color", glm::value_ptr(textinputColor)))
		{
			textinput.textinput.SetTextInputColor(textinputColor);
		}
		if (ImGui::ColorEdit3("Text color", glm::value_ptr(textColor)))
		{
			textinput.textinput.SetTextColor(textColor);
		}

		std::string text = textinput.textinput.GetText();
		if (ImGui::InputText("Text", &text))
		{
			textinput.textinput.SetText(text);
		}

		bool isPasswordField = false;
		bool isNumbersOnly = false;
		if (textinput.textinput.GetFlags() & GUI::GUITextInputFlags::PasswordField)
		{
			isPasswordField = true;
		}

		if (textinput.textinput.GetFlags() & GUI::GUITextInputFlags::NumbersOnly)
		{
			isNumbersOnly = true;
		}

		if (ImGui::Checkbox("Is password field", &isPasswordField))
		{
			auto flags = textinput.textinput.GetFlags();

			if (isPasswordField)
			{
				flags |= GUI::GUITextInputFlags::PasswordField;
			}
			else
			{
				flags &= ~GUI::GUITextInputFlags::PasswordField;
			}
			textinput.textinput.SetFlags(flags);
		}

		if (ImGui::Checkbox("Allow numbers only", &isNumbersOnly))
		{
			auto flags = textinput.textinput.GetFlags();

			if (isNumbersOnly)
			{
				flags |= GUI::GUITextInputFlags::NumbersOnly;
			}
			else
			{
				flags &= ~GUI::GUITextInputFlags::NumbersOnly;
			}
			textinput.textinput.SetFlags(flags);
		}

	}

}