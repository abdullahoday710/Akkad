#include "Shader.h"
#include <fstream>
#include <sstream>

namespace Akkad {
	namespace Graphics {

		ShaderDescriptor Shader::LoadFile(const char* path)
		{
			std::ifstream file(path);
			std::string line;
			std::stringstream vertexsource;
			std::stringstream fragmentsource;
			ShaderProgramType current_type;
			ShaderDescriptor desc;

			while (std::getline(file, line)) {
				
				if (line.find("#VERTEX_SHADER") != std::string::npos)
				{
					current_type = ShaderProgramType::VERTEX;
					desc.ProgramTypes.push_back(ShaderProgramType::VERTEX);
				}

				else if (line.find("#FRAGMENT_SHADER") != std::string::npos)
				{
					current_type = ShaderProgramType::FRAGMENT;
					desc.ProgramTypes.push_back(ShaderProgramType::FRAGMENT);
				}

				else {

					switch (current_type)
					{
					case ShaderProgramType::VERTEX:
					{
						vertexsource << line << "\n";
						break;
					}
					case ShaderProgramType::FRAGMENT:
					{
						fragmentsource << line << "\n";
						break;
					}
					
					}
				}
			}
			desc.VertexSource = vertexsource.str();
			desc.FragmentSource = fragmentsource.str();
			return desc;
		}

	}
}
