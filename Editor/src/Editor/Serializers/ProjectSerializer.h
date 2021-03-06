#pragma once
#include <json.hpp>
#include <filesystem>

#include <Akkad/core.h>

namespace Akkad {

	struct ProjectDescriptor
	{
		nlohmann::json projectData;
		std::filesystem::path ProjectDirectory;
		std::filesystem::path ProjectFilePath;

		std::filesystem::path GetAssetsPath()
		{
			std::filesystem::path result = ProjectDirectory;
			result.append("assets/");
			return result;
		}

		std::string GetScenePath(std::string sceneName)
		{
			bool isNull = projectData["project"]["Scenes"][sceneName].is_null();
			AK_ASSERT(!isNull, "Unkown scene !");

			std::filesystem::path result = ProjectDirectory;
			std::string sceneFile = projectData["project"]["Scenes"][sceneName];
			result.append("assets/scenes/").append(sceneFile);

			return result.string();
		}

		std::filesystem::path GetProjectDirectory()
		{
			std::filesystem::path result(ProjectDirectory);
			return result;
		}
	};

	class ProjectSerializer
	{
	public:
		static ProjectDescriptor NewProject(std::string name, std::string path);
		static ProjectDescriptor LoadProject(std::string path);
		
		static void SaveProject(ProjectDescriptor& desc);
	};

}

