#pragma once
#include "Panel.h"
namespace Akkad {

	class NewProjectPanel : public Panel
	{
	public:
		NewProjectPanel() {};
		~NewProjectPanel() {};

		virtual void DrawImGui() override;
		virtual void OnOpen() override { showPanel = true; }
		virtual void OnClose() override;
		virtual bool IsOpen() override { return showPanel; };

	private:
		static bool showPanel;
	};
}


