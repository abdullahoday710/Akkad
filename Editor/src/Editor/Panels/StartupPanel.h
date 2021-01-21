#pragma once
#include "Panel.h"

namespace Akkad {

	class StartupPanel : public Panel
	{
	public:
		StartupPanel() {};
		~StartupPanel() {};

		virtual void DrawImGui() override;
		virtual void OnOpen() override { showPanel = true; }
		virtual void OnClose() override;
		virtual bool IsOpen() override { return showPanel; };

		virtual std::string GetName() override { return "StartupPanel"; }

	private:
		static bool showPanel;
	};
}


