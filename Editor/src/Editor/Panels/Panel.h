#pragma once
#include <vector>
#include <string>

namespace Akkad {

	class Panel {
	public:
		virtual void DrawImGui() = 0;
		virtual void OnOpen() = 0;
		virtual void OnClose() = 0;
		virtual bool IsOpen() = 0;

		virtual std::string GetName() { return "Panel"; };
	};

	class PanelManager {
	public:

		static void AddPanel(Panel* panel)
		{
			if (!panel->IsOpen())
			{
				s_Panels.push_back(panel);
				panel->OnOpen();
				return;
				
			}

			delete panel;
		}

		static void RemovePanel(Panel* panel)
		{
			panel->OnClose();

			s_Panels.erase(std::remove(s_Panels.begin(), s_Panels.end(), panel), s_Panels.end());
			delete panel;
		}

		static Panel* GetPanel(std::string name)
		{
			auto it = std::find_if(s_Panels.begin(), s_Panels.end(), [&name](Panel* obj) {return obj->GetName() == name; });
			if (it != s_Panels.end())
			{
				return *it;
			}

			else
			{
				return nullptr;
			}
		}

		static void Reset()
		{
			for (auto panel : s_Panels)
			{
				panel->OnClose();
				
				delete panel;
			}
			s_Panels.clear();
		}

		static std::vector<Panel*> GetPanels() { return s_Panels; }
	private:
		static std::vector<Panel*> s_Panels;
	};
}