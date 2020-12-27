#include "ProjectSerializer.h"

#include <filesystem>
#include <fstream>
#include <iomanip>

namespace Akkad {
	using json = nlohmann::json;
	namespace filesystem = std::filesystem;

	ProjectDescriptor ProjectSerializer::NewProject(std::string name, std::string path)
	{
		ProjectDescriptor descriptor;

		json data;
		std::ofstream output;
		data["project"]["name"] = name;

		output.open(path + "/"+name+".AKPROJ");
		output << std::setw(4) << data << std::endl;
		output.close();
		std::string assets = path + "/assets";
		filesystem::create_directory(assets.c_str());
		descriptor.projectData = data;
		descriptor.ProjectPath = path;

		return descriptor;
	}

	ProjectDescriptor ProjectSerializer::LoadProject(std::string path)
	{
		ProjectDescriptor descriptor;

		std::ifstream file;
		file.open(path);
		json data;
		file >> data;
		file.close();
		
		descriptor.projectData = data;
		descriptor.ProjectPath = filesystem::path(path).remove_filename().string();

		return descriptor;
	}

	void ProjectSerializer::SaveProject(ProjectDescriptor& desc)
	{
		std::ofstream output;
		std::string projectName = desc.projectData["project"]["name"];

		output.open(desc.ProjectPath + projectName + ".AKPROJ");
		output << std::setw(4) << desc.projectData << std::endl;
		output.close();
	}
}