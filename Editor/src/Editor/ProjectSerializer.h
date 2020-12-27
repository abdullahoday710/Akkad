#pragma once
#include <json.hpp>

namespace Akkad {

	struct ProjectDescriptor
	{
		nlohmann::json projectData;
		std::string ProjectPath;
	};

	class ProjectSerializer
	{
	public:
		static ProjectDescriptor NewProject(std::string name, std::string path);
		static ProjectDescriptor LoadProject(std::string path);
		
		static void SaveProject(ProjectDescriptor& desc);
	};

}

