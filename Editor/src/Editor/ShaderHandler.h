#pragma once

#include <Akkad/Graphics/Shader.h>
#include <string>
namespace Akkad {

	struct ShaderSourceDescriptor {
		std::vector<Graphics::ShaderProgramType> ProgramTypes;
		std::string VertexSource;
		std::string FragmentSource;

	};

	class ShaderHandler
	{
	public:
		static void CompileSPV(std::string glslpath);
		static ShaderSourceDescriptor LoadSourceFile(const char* path);

	};
}


