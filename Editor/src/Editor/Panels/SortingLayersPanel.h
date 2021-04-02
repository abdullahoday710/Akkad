#pragma once
#include "Panel.h"
namespace Akkad {

	class SortingLayersPanel : public Panel
	{
	public:
		SortingLayersPanel() {};
		~SortingLayersPanel() {};

		virtual void DrawImGui() override;
		virtual void OnOpen() override { showPanel = true; }
		virtual void OnClose() override;
		virtual bool IsOpen() override { return showPanel; };

		virtual std::string GetName() override { return "SortingLayerPanel"; }

	private:
		static bool showPanel;
	};
}


