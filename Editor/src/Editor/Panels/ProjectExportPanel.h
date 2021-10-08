#pragma once

#include "Panel.h"

namespace Akkad {
	class ProjectExportPanel : public Panel {

	public:
		ProjectExportPanel() {};
		~ProjectExportPanel() {};

		virtual void DrawImGui() override;
		virtual void OnOpen() override { showPanel = true; }
		virtual void OnClose() override;
		virtual bool IsOpen() override { return showPanel; };

		virtual std::string GetName() override { return "ProjectExportPanel"; }
		enum class ExportPlatform {WINDOWS, WEB};

	private:
		void WritePackageInfo(std::string exportPath, std::vector<std::string> scenes, std::string startup_scene);
		void CopyRuntimeExecutable(std::string exportPath);
		void CopyAssets(std::string exportPath);
		void CopyGameAssembly(std::string exportPath);

		ExportPlatform m_TargetPlatform = ExportPlatform::WINDOWS;
		std::string ExportPlatformToStr(ExportPlatform platform);
		static bool showPanel;

	};
}