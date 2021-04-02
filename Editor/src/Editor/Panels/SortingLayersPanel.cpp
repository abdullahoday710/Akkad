#include "SortingLayersPanel.h"
#include "Editor/EditorLayer.h"

#include <Akkad/Graphics/SortingLayer2D.h>

#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>
#include <json.hpp>

namespace Akkad {
	bool SortingLayersPanel::showPanel;

	void SortingLayersPanel::DrawImGui()
	{
		if (ImGui::Begin("Sorting Layers", &showPanel))
		{
            auto& layers = SortingLayer2DHandler::GetRegisteredLayers();
            ImGui::BulletText("Drag and drop to re-order sorting layers");
            int move_from = -1, move_to = -1;
            for (int n = 0; n < layers.size(); n++)
            {
                ImGui::Selectable(layers[n].name.c_str());

                ImGuiDragDropFlags src_flags = 0;
                src_flags |= ImGuiDragDropFlags_SourceNoDisableHover;     
                src_flags |= ImGuiDragDropFlags_SourceNoHoldToOpenOthers;
               
                if (ImGui::BeginDragDropSource(src_flags))
                {
                    if (!(src_flags & ImGuiDragDropFlags_SourceNoPreviewTooltip))
                        ImGui::Text("Moving \"%s\"", layers[n].name.c_str());
                    ImGui::SetDragDropPayload("SORTING_LAYER_DRAG_DROP", &n, sizeof(int));
                    ImGui::EndDragDropSource();
                }

                if (ImGui::BeginDragDropTarget())
                {
                    ImGuiDragDropFlags target_flags = 0;
                    target_flags |= ImGuiDragDropFlags_AcceptBeforeDelivery;
                    target_flags |= ImGuiDragDropFlags_AcceptNoDrawDefaultRect;
                    if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SORTING_LAYER_DRAG_DROP", target_flags))
                    {
                        move_from = *(const int*)payload->Data;
                        move_to = n;
                    }
                    ImGui::EndDragDropTarget();
                }
            }
            if (move_from != -1 && move_to != -1)
            {
                std::swap(layers[move_from], layers[move_to]);
                ImGui::SetDragDropPayload("SORTING_LAYER_DRAG_DROP", &move_to, sizeof(int)); // Update payload immediately so on the next frame if we move the mouse to an earlier item our index payload will be correct. This is odd and showcase how the DnD api isn't best presented in this example.
            }
            
            static bool newSortingLayerOpen = false;
            if (!newSortingLayerOpen)
            {
                if (ImGui::Button("Create New sorting layer"))
                {
                    newSortingLayerOpen = true;
                }
            }

            if (newSortingLayerOpen)
            {
                static std::string buf;
                ImGui::InputText("Layer name", &buf);
                if (ImGui::Button("save layer"))
                {
                    if (!buf.empty())
                    {
                        SortingLayer2DHandler::RegisterLayer(buf);
                        buf = "";
                        newSortingLayerOpen = false;
                    }
                }
                
            }

            if (ImGui::Button("Save Layers"))
            {
                nlohmann::ordered_json layerData;

                for (auto it : SortingLayer2DHandler::GetRegisteredLayers())
                {
                    layerData.push_back(it.name);
                }
                
                auto& project = EditorLayer::GetActiveProject();
                project.projectData["project"]["SortingLayers"] = layerData;
                EditorLayer::SaveActiveProject();
            }
            ImGui::Unindent();
		}
		ImGui::End();
	}

	void SortingLayersPanel::OnClose()
	{
		showPanel = false;
	}
}