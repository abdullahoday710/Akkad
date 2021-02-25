#pragma once
#include <Akkad/Graphics/Shader.h>

#include <string>
#include <filesystem>
namespace Akkad {

	struct ShaderSourceDescriptor {
		std::vector<Graphics::ShaderProgramType> ProgramTypes;
		std::string VertexSource;
		std::string FragmentSource;

	};

	class ShaderHandler
	{
	public:
		static void CompileSPV(std::filesystem::path glslpath, std::filesystem::path outputPath);
		static ShaderSourceDescriptor LoadSourceFile(const char* path);

	};
}


