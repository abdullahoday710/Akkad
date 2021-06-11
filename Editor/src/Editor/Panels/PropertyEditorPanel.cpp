#include "PropertyEditorPanel.h"
#include "MaterialEditorPanel.h"

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

				if (ImGui::Button("Rigidbody 2D"))
				{
					if (!m_ActiveEntity.HasComponent<RigidBody2dComponent>())
					{
						m_ActiveEntity.AddComponent<RigidBody2dComponent>();
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
			ImGui::DragFloat3("Position", glm::value_ptr(transform.GetPosition()));
			ImGui::DragFloat3("Rotation", glm::value_ptr(transform.GetRotation()));
			ImGui::DragFloat3("Scale", glm::value_ptr(transform.GetScale()));
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
			bool single_texture = sprite.sprite.IsSingleTexture();
			if(ImGui::Checkbox("Is single texture", &single_texture))
			{
				sprite.sprite.UseSingleTexture(single_texture);
			}

			if (!sprite.sprite.IsSingleTexture())
			{
				glm::vec2 tileSize = sprite.sprite.GetTileSize();
				if (ImGui::InputFloat2("tile size", glm::value_ptr(tileSize)))
				{
					sprite.sprite.SetTileSize(tileSize);
				}
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
			}
			
			ImGui::TreePop();
		}
	}

	void PropertyEditorPanel::DrawCameraComponent()
	{
		ImGui::SetNextItemOpen(true);
		if (ImGui::TreeNode("Camera"))
		{
			if (DrawComponentContextMenu<CameraComponent>(m_ActiveEntity))
			{
				ImGui::TreePop();
				return;
			}
			auto& camera = m_ActiveEntity.GetComponent<CameraComponent>();
			ImGui::Text("Camera :");
			ImGui::Checkbox("Active", &camera.isActive);
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
				if (DrawComponentContextMenu<ScriptComponent>(m_ActiveEntity))
				{
					ImGui::TreePop();
					return;
				}

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

			const char* body_types[] = { "Static", "Dynamic" };
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
					}
					if (is_selected)
					{
						ImGui::SetItemDefaultFocus();
					}
				}
				ImGui::EndCombo();
			}

			if (current_body_type == "Static")
			{
				rigidBody.type = BodyType::STATIC;
			}
			if (current_body_type == "Dynamic")
			{
				rigidBody.type = BodyType::DYNAMIC;
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

		}
		ImGui::TreePop();
	}

	void PropertyEditorPanel::DrawGUIContainerComponent()
	{
		ImGui::SetNextItemOpen(true);
		auto& containerComponent = m_ActiveEntity.GetComponent<GUIContainerComponent>();
		glm::vec2 size = containerComponent.container.GetScreenSize();
		if (ImGui::TreeNode("container"))
		{
			ImGui::InputFloat2("size", glm::value_ptr(size));
		}
		ImGui::TreePop();
	}

	std::string ConstraintTypeToStr_(GUI::ConstraintType type)
	{
		switch (type)
		{
		case GUI::ConstraintType::CENTER_CONSTRAINT:
			return "Center";
		case GUI::ConstraintType::RELATIVE_CONSTRAINT:
			return "Relative";
		case GUI::ConstraintType::PIXEL_CONSTRAINT:
			return "Pixel";
		case GUI::ConstraintType::ASPECT_CONSTRAINT:
			return "Aspect";
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

			const char* position_constraints[] = { "Relative", "Pixel", "Center" };
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
		}
		ImGui::TreePop();
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
		}
		ImGui::TreePop();
	}

}