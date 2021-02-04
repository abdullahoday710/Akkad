#include "ProjectSerializer.h"

#include <Akkad/Random.h>
#include <Akkad/Logging.h>
#include <Akkad/Application/Application.h>
#include <Akkad/Asset/AssetManager.h>

#include <fstream>
#include <iomanip>

namespace Akkad {
	using json = nlohmann::json;
	namespace filesystem = std::filesystem;

	ProjectDescriptor ProjectSerializer::NewProject(std::string name, std::string path)
	{
		ProjectDescriptor descriptor;

		json data;
		data["project"]["name"] = name;

		std::string assets = path + "/assets";
		std::string scenes = assets + "/scenes";

		filesystem::create_directory(assets.c_str());
		filesystem::create_directory(scenes.c_str());

		descriptor.projectData = data;
		descriptor.ProjectDirectory = path + "/";

		filesystem::path GameAssemblyPath = filesystem::current_path();
		filesystem::path EngineResourcesPath = filesystem::current_path();

		GameAssemblyPath += "/GameAssembly";
		EngineResourcesPath += "/res";

		filesystem::copy(GameAssemblyPath, path + "/GameAssembly", filesystem::copy_options::recursive);
		filesystem::copy(EngineResourcesPath, path + "/res", filesystem::copy_options::recursive);

		for (auto& file : filesystem::directory_iterator(EngineResourcesPath))
		{
			if (file.is_directory())
			{
				for (auto& subdir : filesystem::directory_iterator(file.path()))
				{
					filesystem::copy(subdir.path(), path + "/assets");

					std::string assetName = subdir.path().filename().string();
					std::string assetID = Random::GenerateRandomUUID();

					descriptor.projectData["project"]["Assets"][assetID]["path"] = "assets/" + assetName;
					descriptor.projectData["project"]["Assets"][assetID]["name"] = assetName;

					AssetDescriptor assetDesc;
					assetDesc.absolutePath = path + "/assets/" + assetName;

					Application::GetAssetManager()->RegisterAsset(assetID, assetDesc);
				}
			}
		}

		descriptor.ProjectFilePath = path + "/" + name + ".AKPROJ";

		std::ofstream output;
		output.open(path + "/" + name + ".AKPROJ");
		output << std::setw(4) << descriptor.projectData << std::endl;
		output.close();

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


		Application::GetAssetManager()->Clear();

		for (auto& asset : descriptor.projectData["project"]["Assets"].items())
		{
			std::string assetID = asset.key();
			std::string assetName = descriptor.projectData["project"]["Assets"][assetID]["name"];
			std::string absolutePath = descriptor.GetAssetsPath().string() + assetName;

			AssetDescriptor descriptor;
			descriptor.absolutePath = absolutePath;

			Application::GetAssetManager()->RegisterAsset(assetID, descriptor);
		}

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