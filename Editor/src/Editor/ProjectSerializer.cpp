#include "ProjectSerializer.h"

#include <Akkad/Logging.h>

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
		std::string scenes = assets + "/scenes";

		filesystem::create_directory(assets.c_str());
		filesystem::create_directory(scenes.c_str());
		descriptor.projectData = data;
		descriptor.ProjectDirectory = path;

		descriptor.ProjectFilePath = path + "/" + name + ".AKPROJ";

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
		descriptor.ProjectDirectory = filesystem::path(path).remove_filename().string();
		descriptor.ProjectFilePath = path;

		return descriptor;
	}

	void ProjectSerializer::SaveProject(ProjectDescriptor& desc)
	{
		std::ofstream output;

		output.open(desc.ProjectFilePath);
		output << std::setw(4) << desc.projectData << std::endl;
		output.close();
	}
}