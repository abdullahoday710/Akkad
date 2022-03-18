#include "ProjectExportPanel.h"
#include "Editor/EditorLayer.h"

#include <Akkad/PlatformUtils.h>
#include <Akkad/Graphics/SortingLayer2D.h>

#include <json.hpp>
#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>

#include <fstream>
namespace Akkad {
	bool ProjectExportPanel::showPanel;
	void ProjectExportPanel::DrawImGui()
	{
		ImGui::Begin("Export project", &showPanel);
		static std::string export_path;
		ImGui::InputText("Select export path", &export_path);

		if (ImGui::Button("Browse"))
		{
			export_path = PlatformUtils::OpenDirectoryDialog();
		}

		auto& project = EditorLayer::GetActiveProject();

		static std::vector<std::string> selected_scenes;
		static std::string startup_scene;
		if (ImGui::ListBoxHeader("Scenes to export"))
		{
			unsigned int id = 0;
			for (auto& scene : project.projectData["project"]["Scenes"].items())
			{
				std::string scene_value = scene.value();
				bool is_selected = std::find(selected_scenes.begin(), selected_scenes.end(), scene_value) != selected_scenes.end();
				ImGui::Button(scene.key().c_str());
				ImGui::SameLine();

				ImGui::PushID(id);
				if (ImGui::Checkbox("", &is_selected))
				{
					if (is_selected)
					{
						selected_scenes.push_back(scene_value);
					}
					else
					{
						auto it = std::find(selected_scenes.begin(), selected_scenes.end(), scene_value);
						selected_scenes.erase(it);
					}
				}
				ImGui::PopID();

				bool isStartupScene = false;

				if (is_selected && startup_scene == scene_value)
				{
					isStartupScene = true;
				}
				
				ImGui::SameLine();
				ImGui::PushID(id + 1);
				if (ImGui::Checkbox("startup scene", &isStartupScene))
				{
					if (is_selected)
					{
						startup_scene = scene_value;
					}
				}
				ImGui::PopID();
				id += 2;
			}
			ImGui::ListBoxFooter();
		}

		static const char* Platforms[] = { "Windows", "Web" };
		auto current_platform_str = ExportPlatformToStr(m_TargetPlatform);
		static const char* selected_platform = current_platform_str.c_str();

		if (ImGui::BeginCombo("Platform", selected_platform))
		{
			for (int n = 0; n < IM_ARRAYSIZE(Platforms); n++)
			{
				bool is_selected = (selected_platform == Platforms[n]);
				if (ImGui::Selectable(Platforms[n], is_selected))
				{
					selected_platform = Platforms[n];
					if (selected_platform == "Windows")
					{
						m_TargetPlatform = ExportPlatform::WINDOWS;
					}
					else if (selected_platform == "Web")
					{
						m_TargetPlatform = ExportPlatform::WEB;
					}
	
				}
				if (is_selected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}

		if (ImGui::Button("Begin export"))
		{
			if (!export_path.empty())
			{
				WritePackageInfo(export_path, selected_scenes, startup_scene);
				CopyRuntimeExecutable(export_path);
				CopyGameAssembly(export_path);
				CopyAssets(export_path);

			}
		}
		ImGui::End();
	}

	void ProjectExportPanel::OnClose()
	{
	}

	void ProjectExportPanel::WritePackageInfo(std::string exportPath, std::vector<std::string> scenes, std::string startup_scene)
	{
		auto& project = EditorLayer::GetActiveProject();

		nlohmann::json package_info;

		package_info["name"] = project.projectData["project"]["name"];

		/* removing the file extension from the startup scene name */
		size_t lastindex = startup_scene.find_last_of(".");
		std::string rawname = startup_scene.substr(0, lastindex);

		package_info["startupScene"] = rawname;

		for (auto sceneName : scenes)
		{
			package_info["Scenes"].push_back(sceneName);
		}

		for (auto asset : project.projectData["project"]["Assets"].items())
		{
			package_info["Assets"][asset.key()] = asset.value();
		}

		for (auto layer : SortingLayer2DHandler::GetRegisteredLayers())
		{
			package_info["SortingLayers2D"].push_back(layer.name);
		}

		std::vector<std::uint8_t> bson = nlohmann::json::to_cbor(package_info);

		std::ofstream outPackageInfo;

		std::string outputPackageInfoPath = exportPath + "/package_info.akpkg";

		outPackageInfo.open(outputPackageInfoPath, std::ios::trunc | std::ios::binary);

		for (auto value : bson)
		{
			outPackageInfo << value;
		}
		outPackageInfo.close();
	}

	void ProjectExportPanel::CopyRuntimeExecutable(std::string exportPath)
	{
		auto& project = EditorLayer::GetActiveProject();

		std::string projectName = project.projectData["project"]["name"];
		if (m_TargetPlatform == ExportPlatform::WINDOWS)
		{
			std::string srcpath = "Runtime/Runtime.exe";
			std::string destpath = exportPath + "/" + projectName + ".exe";

			if (std::filesystem::exists(destpath))
			{
				std::filesystem::remove(destpath);
			}
			std::filesystem::copy(srcpath, destpath);
		}

		else if (m_TargetPlatform == ExportPlatform::WEB)
		{
			std::string htmlsrcpath = "Runtime/Web/Runtime.html";
			std::string htmldestpath = exportPath + "/" + projectName + ".html";
			
			std::string jsSrcPath = "Runtime/Web/Runtime.js";
			std::string jsDestPath = exportPath + "/" + "Runtime.js";

			std::string wasmSrcPath = "Runtime/Web/Runtime.wasm";
			std::string wasmDestPath = exportPath + "/" + "Runtime.wasm";

			if (std::filesystem::exists(htmldestpath))
			{
				std::filesystem::remove(htmldestpath);
			}

			if (std::filesystem::exists(wasmDestPath))
			{
				std::filesystem::remove(wasmDestPath);
			}

			if (std::filesystem::exists(jsDestPath))
			{
				std::filesystem::remove(jsDestPath);
			}

			//std::filesystem::copy(htmlsrcpath, htmldestpath);
			std::filesystem::copy(jsSrcPath, jsDestPath);
			std::filesystem::copy(wasmSrcPath, wasmDestPath);
		}


	}

	void ProjectExportPanel::CopyAssets(std::string exportPath)
	{
		auto& project = EditorLayer::GetActiveProject();
		if (m_TargetPlatform == ExportPlatform::WINDOWS)
		{
			std::string assetsSrcPath = project.GetAssetsPath().string();
			std::string assetsDestPath = exportPath + "/assets";

			if (std::filesystem::exists(assetsDestPath))
			{
				std::filesystem::remove_all(assetsDestPath);
			}
			std::filesystem::copy(assetsSrcPath, assetsDestPath, std::filesystem::copy_options::recursive);
		}

		if (m_TargetPlatform == ExportPlatform::WEB)
		{
			std::string emsdk_path = std::getenv("AK_EMSDK");
			std::string file_packager_path = emsdk_path + "/upstream/emscripten/file_packager.py ";

			std::string assets_preload_export_path = exportPath + "/assets_preload ";
			std::string package_info_preload = "--preload "+exportPath+"/package_info.akpkg@/ ";
			std::string game_assembly_preload = "--preload " + project.GetProjectDirectory().string() + "GameAssembly/build/GameAssembly.js@/ ";
			std::string assets_preload = "--preload " + project.GetAssetsPath().string() + "@assets ";
			std::string js_output = "--js-output=" + exportPath + "/assets_preload.js ";

			std::string package_files_command = file_packager_path + assets_preload_export_path + package_info_preload + game_assembly_preload + assets_preload + js_output;
			system(package_files_command.c_str());
		}
		

	}

	void ProjectExportPanel::CopyGameAssembly(std::string exportPath)
	{
		auto& project = EditorLayer::GetActiveProject();
		std::string assemblySrcPath = project.GetProjectDirectory().string();
		std::string assemblyDestPath;

		if (m_TargetPlatform == ExportPlatform::WINDOWS)
		{
			assemblySrcPath += "/GameAssembly/build/GameAssembly.dll";

			assemblyDestPath = exportPath + "/GameAssembly.dll";

			if (std::filesystem::exists(assemblyDestPath))
			{
				std::filesystem::remove(assemblyDestPath);
			}
			std::filesystem::copy(assemblySrcPath, assemblyDestPath);

		}

		else if (m_TargetPlatform == ExportPlatform::WEB)
		{
			std::string compilePath = assemblySrcPath + "GameAssembly";
			std::string compileCommand = "cd " + compilePath + "&& ninja";
			system(compileCommand.c_str());

		}


	}
	std::string ProjectExportPanel::ExportPlatformToStr(ExportPlatform platform)
	{
		switch (platform)
		{
		case ExportPlatform::WINDOWS:
			return "Windows";
		case ExportPlatform::WEB:
			return "Web";
		}
	}
}