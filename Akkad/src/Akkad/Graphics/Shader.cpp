#include "Shader.h"

#include <Akkad/Application/Application.h>
#include <Akkad/Asset/AssetManager.h>

#include <fstream>
#include <sstream>

namespace Akkad {
	namespace Graphics {

		std::vector<unsigned int> Shader::LoadSpirV(const char* spirvPath)
		{
			std::vector<unsigned int> spirv_data;

			std::ifstream spirv_file;
			spirv_file.open(spirvPath, std::ios::in);

			std::string line;
			while (std::getline(spirv_file, line))
			{
				std::istringstream iss(line);
				unsigned int value;
				if (!(iss >> value)) { break; }

				spirv_data.push_back(value);
			}

			return spirv_data;
		}

		ShaderDescriptor Shader::LoadShader(const char* shaderdescPath)
		{
			std::ifstream file(shaderdescPath);
			std::string line;
			std::stringstream vertexPath;
			std::stringstream fragmentPath;

			ShaderProgramType current_type;
			ShaderDescriptor desc;

			while (std::getline(file, line)) {

				if (!line.empty())
				{
					if (line.find("#VERTEX_SHADER") != std::string::npos)
					{
						current_type = Graphics::ShaderProgramType::VERTEX;
						desc.ProgramTypes.push_back(ShaderProgramType::VERTEX);
					}

					else if (line.find("#FRAGMENT_SHADER") != std::string::npos)
					{
						current_type = Graphics::ShaderProgramType::FRAGMENT;
						desc.ProgramTypes.push_back(ShaderProgramType::FRAGMENT);
					}

					else {

						switch (current_type)
						{
						case ShaderProgramType::VERTEX:
						{
							vertexPath << line;
							break;
						}
						case ShaderProgramType::FRAGMENT:
						{
							fragmentPath << line;
							break;
						}

						}
					}
			}

			}

			std::string assetRootPath = Application::GetAssetManager()->GetAssetsRootPath() + "compiledSPV/";

			std::string vertexAbsolutePath = assetRootPath + vertexPath.str();
			std::string fragmentAbsolutePath = assetRootPath + fragmentPath.str();

			desc.VertexData = LoadSpirV(vertexAbsolutePath.c_str());
			desc.FragmentData = LoadSpirV(fragmentAbsolutePath.c_str());

			return desc;
		}

	}
}
