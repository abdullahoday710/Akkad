#pragma once
#include <json.hpp>
#include <filesystem>

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
	};

	class ProjectSerializer
	{
	public:
		static ProjectDescriptor NewProject(std::string name, std::string path);
		static ProjectDescriptor LoadProject(std::string path);
		
		static void SaveProject(ProjectDescriptor& desc);
	};

}

